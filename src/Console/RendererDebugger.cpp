#include <iomanip>
#include <time.h>

#include "RendererDebugger.hpp"
#include "../GeoGen/utils/StringUtils.hpp"
#include "ConsoleUtils.hpp"
#include "renderer_commands/AutosaveRendererCommand.hpp"
#include "renderer_commands/DumpRendererCommand.hpp"
#include "renderer_commands/HelpRendererCommand.hpp"
#include "renderer_commands/ObjectTableRendererCommand.hpp"
#include "renderer_commands/QuitRendererCommand.hpp"
#include "renderer_commands/RenderingSequenceRendererCommand.hpp"
#include "renderer_commands/RenderingSequenceGraphRendererCommand.hpp"
#include "renderer_commands/RenderingSequenceMetadataRendererCommand.hpp"
#include "renderer_commands/RunRendererCommand.hpp"
#include "renderer_commands/SaveRendererCommand.hpp"
#include "renderer_commands/StepRendererCommand.hpp"
#include "renderer_commands/StopRendererCommand.hpp"

using namespace geogen;
using namespace compiler;
using namespace runtime;
using namespace console;
using namespace renderer;
using namespace utils;
using namespace std;
using namespace renderer_commands;
using namespace instructions;

RendererDebugger::RendererDebugger(geogen::IStream& in, geogen::OStream& out, renderer::RenderingSequence const& sequence, String outputDirectory)
: renderer(sequence), in(in), out(out), outputDirectory(outputDirectory)
{
	this->commandTable.AddCommand(new AutosaveRendererCommand());
	this->commandTable.AddCommand(new DumpRendererCommand());
	this->commandTable.AddCommand(new HelpRendererCommand());
	this->commandTable.AddCommand(new QuitRendererCommand());
	this->commandTable.AddCommand(new ObjectTableRendererCommand());
	this->commandTable.AddCommand(new RenderingSequenceRendererCommand());
	this->commandTable.AddCommand(new RenderingSequenceGraphRendererCommand());
	this->commandTable.AddCommand(new RenderingSequenceMetadataRendererCommand());
	this->commandTable.AddCommand(new RunRendererCommand());
	this->commandTable.AddCommand(new SaveRendererCommand());
	this->commandTable.AddCommand(new StepRendererCommand());
	this->commandTable.AddCommand(new StopRendererCommand());

}

void RendererDebugger::Run()
{
	out << std::endl << GG_STR("Starting renderer debugger. ") << std::endl;

	this->GetRenderer()->CalculateMetadata();

	out << std::endl << GG_STR("Calculated metadata. ") << std::endl << std::endl;

	renderer::RenderingStep const* currentStep = this->renderer.GetNextRenderingStep();
	out << GG_STR("Next step: \"") << (currentStep->ToString()) << GG_STR("\" on line ") << currentStep->GetLocation().GetLine() << GG_STR(", column ") << currentStep->GetLocation().GetColumn() << GG_STR(". ") << std::endl << std::endl;

	String input = "";
	while (renderer.GetStatus() == RENDERER_STATUS_READY)
	{
		out << GG_STR("RENDERER>> ");

		getline<Char>(in, input);

		size_t separatorPosition = input.find(" ");
		string commandCue = input.substr(0, separatorPosition);
		string args = "";
		if (separatorPosition != String::npos)
		{
			args = input.substr(separatorPosition + 1);
		}

		Command const* command = this->commandTable.GetCommand(commandCue);
		if (command == NULL)
		{
			out << GG_STR("Unknown command. Enter \"?\" to print list of commands available in current context.") << endl << endl;
		}
		else
		{
			dynamic_cast<RendererCommand const*>(command)->Run(this, args);
		}

		if (this->aborted)
		{
			out << GG_STR("Renderer debugger aborted.") << endl << endl;
			return;
		}
	}

	out << GG_STR("Renderer debugger finished.") << endl << endl;
}

void RendererDebugger::Step()
{
	RenderingStep const* step = this->GetRenderer()->GetNextRenderingStep();

	clock_t startTime = clock();

	this->renderer.Step();

	this->lastStepTime = (double)(clock() - startTime) / (double)CLOCKS_PER_SEC;

	if (this->autosave)
	{
		RendererObject* returnObject = this->renderer.GetObjectTable().GetObjectBySlot(step->GetReturnSlot());

		String stepName = step->GetName();
		std::replace(stepName.begin(), stepName.end(), '.', '-');

		StringStream ss;
		ss << this->GetOutputDirectory() << GG_STR("/step") << this->currentStepNumber << GG_STR("_") << stepName << (returnObject->GetObjectType() == RENDERER_OBJECT_TYPE_HEIGHT_MAP ? GG_STR(".png") : GG_STR(".csv"));

		try
		{
			WriteImage(returnObject->GetPtr(), returnObject->GetObjectType(), ss.str());

			this->GetOut() << GG_STR("Saved step result as \"") << ss.str() << ("\".") << std::endl;
		}
		catch (exception&)
		{
			this->GetOut() << GG_STR("Could not write \"") << ss.str() << ("\".") << std::endl;
		}
	}

	this->currentStepNumber++;
}