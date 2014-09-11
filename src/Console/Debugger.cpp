#include <iomanip>

#include "Debugger.hpp"
#include <GeoGen/GeoGen.hpp>
#include "ConsoleUtils.hpp"
#include "VirtualMachineCallback.hpp"
#include "SignalHandler.hpp"
#include "runtime_commands/ArgumentsRuntimeCommand.hpp"
#include "runtime_commands/CallStackRuntimeCommand.hpp"
#include "runtime_commands/CodeBlockCodeRuntimeCommand.hpp"
#include "runtime_commands/CodeBlockStackRuntimeCommand.hpp"
#include "runtime_commands/DumpRuntimeCommand.hpp"
#include "runtime_commands/HelpRuntimeCommand.hpp"
#include "runtime_commands/ManagedObjectsRuntimeCommand.hpp"
#include "runtime_commands/ObjectStackRuntimeCommand.hpp"
#include "runtime_commands/QuitRuntimeCommand.hpp"
#include "runtime_commands/ReadVariableRuntimeCommand.hpp"
#include "runtime_commands/RenderingSequenceRuntimeCommand.hpp"
#include "runtime_commands/RunRuntimeCommand.hpp"
#include "runtime_commands/StepRuntimeCommand.hpp"
#include "runtime_commands/StopRuntimeCommand.hpp"
#include "runtime_commands/SupportedMapsRuntimeCommand.hpp"

using namespace geogen;
using namespace compiler;
using namespace runtime;
using namespace console;
using namespace renderer;
using namespace utils;
using namespace std;
using namespace runtime_commands;
using namespace instructions;

const unsigned Debugger::NUMBER_OF_SHOWN_LINES_DEFAULT = 7;

Debugger::Debugger(geogen::IStream& in, geogen::OStream& out, runtime::CompiledScript const& code, runtime::ScriptParameters parameters, String outputDirectory)
: vm(code, parameters), in(in), out(out), outputDirectory(outputDirectory)
{
	vm.SetCallbackData(&out);
	vm.SetScriptMessageHandler(VirtualMachineCallback);

	commandTable.AddCommand(new ArgumentsRuntimeCommand());
	commandTable.AddCommand(new CallStackRuntimeCommand());
	commandTable.AddCommand(new CodeBlockCodeRuntimeCommand());
	commandTable.AddCommand(new CodeBlockStackRuntimeCommand());
	commandTable.AddCommand(new DumpRuntimeCommand());
	commandTable.AddCommand(new HelpRuntimeCommand());
	commandTable.AddCommand(new ManagedObjectsRuntimeCommand());
	commandTable.AddCommand(new ObjectStackRuntimeCommand());
	commandTable.AddCommand(new QuitRuntimeCommand());
	commandTable.AddCommand(new ReadVariableRuntimeCommand());
	commandTable.AddCommand(new RenderingSequenceRuntimeCommand());
	commandTable.AddCommand(new RunRuntimeCommand());
	commandTable.AddCommand(new StepRuntimeCommand());
	commandTable.AddCommand(new StopRuntimeCommand());
	commandTable.AddCommand(new SupportedMapsRuntimeCommand());
}

void Debugger::ShowCodeContext() const
{
	vector<String> codeLines = geogen::utils::StringToLines(this->vm.GetCompiledScript().GetCode());

	if (this->numberOfShownLines > 0)
	{//codeLines.at(max(currentInstruction->GetLocation().GetLine(), 0)
		Instruction const* currentInstruction = this->GetCurrentInstruction();

		if (this->GetVirtualMachine()->GetStatus() == VIRTUAL_MACHINE_STATUS_FINISHED)
		{
			return;
		}

		for (
			int i = -(int)numberOfShownLines;
			i < (int)numberOfShownLines;
			i++
		)
		{
			int currentLineNumber = currentInstruction->GetLocation().GetLine() + i;
			if (currentLineNumber >= 0 && currentLineNumber < (int)codeLines.size())
			{
				String currentLine = codeLines[currentLineNumber];

				cout
					<< std::setw(4)
					<< currentLineNumber << " "
					<< std::setw(4) << (currentLineNumber + 1 == currentInstruction->GetLocation().GetLine() ? ">>>" : "") << " ";

				if (currentLineNumber + 1 == currentInstruction->GetLocation().GetLine())
				{
					Cout << currentLine.substr(0, currentInstruction->GetLocation().GetColumn());
					HighlightYellow();
					Cout << currentLine[currentInstruction->GetLocation().GetColumn()];
					Unhighlight();
					if (currentLine.length() > 0)
					{
						Cout << currentLine.substr(currentInstruction->GetLocation().GetColumn() + 1);
					}
				}
				else
				{
					Cout << currentLine;
				}
			}

			Cout << std::endl;

			//	<< *it << std::endl;
		}
	}
}

runtime::instructions::Instruction const* Debugger::GetCurrentInstruction() const
{
	if (this->vm.GetCallStack().IsEmpty()) return NULL;

	runtime::CallStackEntry const& callStackTop = this->vm.GetCallStack().Top();

	if (callStackTop.GetCodeBlockStack().IsEmpty()) return NULL;

	runtime::CodeBlockStackEntry const& codeBlockStackTop = callStackTop.GetCodeBlockStack().Top();

	return codeBlockStackTop.GetCurrentInstruction();
};

void Debugger::Run()
{
	this->ShowCodeContext();

	runtime::instructions::Instruction const* currentInstruction = this->GetCurrentInstruction();
	out << std::endl << GG_STR("Next instruction: \"") << (currentInstruction->ToString()) << GG_STR("\" on line ") << currentInstruction->GetLocation().GetLine() << GG_STR(", column ") << currentInstruction->GetLocation().GetColumn() << GG_STR(". ") << std::endl << std::endl;

	String input = "";
	while (vm.GetStatus() == VIRTUAL_MACHINE_STATUS_READY)
	{
		out << GG_STR("DEBUGGER>> ");

		if (!getline<Char>(in, input))
		{
			IgnoreNextSignal();
			cout << std::endl;
			this->Abort();
		}

		in.clear();
		//in.ignore(10000, '\n');

		size_t separatorPosition = input.find(" ");
		string commandCue = input.substr(0, separatorPosition);
		string args = "";
		if (separatorPosition != String::npos)
		{
			args = input.substr(separatorPosition + 1);
		}

		if (GetAndClearAbortFlag() || this->aborted)
		{
			this->Abort();
		}
		else 
		{
			Command const* command = this->commandTable.GetCommand(commandCue);
			if (command == NULL)
			{
				out << GG_STR("Unknown command. Enter \"?\" to print list of commands available in current context.") << endl << endl;
			}
			else
			{
				dynamic_cast<RuntimeCommand const*>(command)->Run(this, args);
			}
		}

		if (this->aborted)
		{
			out << GG_STR("Debugger aborted.") << endl << endl;
			return;
		}
	}

	out << GG_STR("Debugger finished.") << endl;
}