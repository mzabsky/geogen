#include <iomanip>

#include "Loader.hpp"
#include "../GeoGen/utils/StringUtils.hpp"
#include "ConsoleUtils.hpp"
#include "ImageWriter.hpp"
#include "loader_commands/DebugLoaderCommand.hpp"
#include "loader_commands/HelpLoaderCommand.hpp"
#include "loader_commands/LoadLoaderCommand.hpp"
#include "loader_commands/MapSizeLoaderCommand.hpp"
#include "loader_commands/QuitLoaderCommand.hpp"
#include "loader_commands/RandomSeedLoaderCommand.hpp"
#include "loader_commands/ReloadLoaderCommand.hpp"
#include "loader_commands/RenderScaleLoaderCommand.hpp"
#include "loader_commands/RenderSizeLoaderCommand.hpp"
#include "loader_commands/RunLoaderCommand.hpp"

using namespace geogen;
using namespace compiler;
using namespace runtime;
using namespace console;
using namespace renderer;
using namespace utils;
using namespace std;
using namespace loader_commands;
using namespace instructions;

Loader::Loader(geogen::IStream& in, geogen::OStream& out, ProgramArguments programArguments)
: currentFile(programArguments.inputFile), outputDirectory(programArguments.outputDirectory), debug(debug), in(in), out(out), renderOrigin(0, 0), renderSize(MAP_SIZE_AUTOMATIC, MAP_SIZE_AUTOMATIC), mapSize(MAP_SIZE_AUTOMATIC, MAP_SIZE_AUTOMATIC), renderScale(1)
{
	this->commandTable.AddCommand(new DebugLoaderCommand());
	this->commandTable.AddCommand(new HelpLoaderCommand());
	this->commandTable.AddCommand(new LoadLoaderCommand());
	this->commandTable.AddCommand(new MapSizeLoaderCommand());
	this->commandTable.AddCommand(new QuitLoaderCommand());
	this->commandTable.AddCommand(new RandomSeedLoaderCommand());
	this->commandTable.AddCommand(new ReloadLoaderCommand());
	this->commandTable.AddCommand(new RenderScaleLoaderCommand());
	this->commandTable.AddCommand(new RenderSizeLoaderCommand());
	this->commandTable.AddCommand(new RunLoaderCommand());
}

void Loader::Run()
{	
	if (this->currentFile != GG_STR(""))
	{
		LoadLoaderCommand().Run(this, this->currentFile);
	}

	String input = "";
	while (true)
	{
		out << GG_STR("LOADER>> ");

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
			out << GG_STR("Unknown command. Use \"h\" to print list of available commands.") << endl << endl;
		}
		else
		{
			try
			{
				dynamic_cast<LoaderCommand const*>(command)->Run(this, args);
			}
			catch (GeoGenException& e)
			{
				HighlightRed();
				out << "Error GGE" << e.GetErrorCode() << ": " << e.GetDetailMessage() << endl;
				Unhighlight();

				if (this->GetDump() != GG_STR(""))
				{
					out << GG_STR("Do you wish to save dump file? (y/n, default = n) ");
					String saveDumpResponse;
					getline(Cin, saveDumpResponse);

					if (saveDumpResponse == GG_STR("y") || saveDumpResponse == GG_STR("Y") || saveDumpResponse == GG_STR("yes"))
					{
						out << GG_STR("Dump file name (default = dump.txt): ");
						String dumpFileName;
						getline(Cin, dumpFileName);

						if (dumpFileName == GG_STR(""))
						{
							dumpFileName = "dump.txt";
						}

						OFStream dumpStream(dumpFileName);
						dumpStream << this->GetDump();
						dumpStream.flush();

						if (dumpStream.fail())
						{
							out << GG_STR("Could not write dump file.") << std::endl << std::endl;
						}
						else
						{
							out << GG_STR("Saved dump file \"") << dumpFileName << GG_STR("\".") << std::endl << std::endl;
						}
					}
					else
					{
						out << GG_STR("Discarded dump file.") << std::endl << std::endl;
					}

					this->SetDump(GG_STR(""));
				}
			}
		}
	}
}

void Loader::SaveRenderedMaps(renderer::RenderedMapTable& renderedMaps)
{
	for (RenderedMapTable::iterator it = renderedMaps.Begin(); it != renderedMaps.End(); it++)
	{
		stringstream ss;
		ss << this->outputDirectory << GG_STR("/") << StringToAscii(it->first) << GG_STR(".png");

		bool success = true;
		try
		{
			WriteImage(it->second, RENDERER_OBJECT_TYPE_HEIGHT_MAP, ss.str());
		}
		catch (exception&)
		{
			success = false;
			out << GG_STR("Could not save \"") << ss.str() << "\"." << endl;
		}

		if (success)
		{
			out << GG_STR("Saved \"") << ss.str() << "\"." << endl;
		}		
	}
	renderedMaps.Clear();

	out << endl;
}

ScriptParameters Loader::CreateScriptParameters()
{
	ScriptParameters params = this->GetCompiledScript()->CreateScriptParameters();

	params.SetRenderHeight(this->GetRenderSize().GetWidth());
	params.SetRenderWidth(this->GetRenderSize().GetHeight());
	params.SetMapWidth(this->GetMapSize().GetWidth());
	params.SetMapHeight(this->GetMapSize().GetHeight());
	params.SetRenderScale(this->GetRenderScale());
	params.SetRandomSeed(random::RecognizeSeed(this->randomSeed));

	return params;
}