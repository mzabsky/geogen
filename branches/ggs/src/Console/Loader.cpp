#include <iomanip>

#include "../png++/png.hpp"

#include "Loader.hpp"
#include "../GeoGen/utils/StringUtils.hpp"
#include "ConsoleUtils.hpp"
#include "loader_commands/DebugLoaderCommand.hpp"
#include "loader_commands/HelpLoaderCommand.hpp"
#include "loader_commands/LoadLoaderCommand.hpp"
#include "loader_commands/QuitLoaderCommand.hpp"
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
: currentFile(programArguments.inputFile), outputDirectory(programArguments.outputDirectory), debug(debug), in(in), out(out)
{
	this->commandTable.AddCommand(new DebugLoaderCommand());
	this->commandTable.AddCommand(new HelpLoaderCommand());
	this->commandTable.AddCommand(new LoadLoaderCommand());
	this->commandTable.AddCommand(new QuitLoaderCommand());
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
		png::image<png::ga_pixel_16>image(it->second->GetRectangle().GetSize().GetWidth(), it->second->GetRectangle().GetSize().GetHeight());
		for (size_t y = 0; y < image.get_height(); ++y)
		{
			for (size_t x = 0; x < image.get_width(); ++x)
			{
				image[y][x] = png::ga_pixel_16((unsigned short)((long)-HEIGHT_MIN + (long)(*it->second)(x, y)));
			}
		}

		stringstream ss;
		ss << this->outputDirectory << GG_STR("/") << StringToAscii(it->first) << GG_STR(".png");
		image.write(ss.str());

		out << GG_STR("Saved \"") << ss.str() << "\"." << endl;
	}
	renderedMaps.Clear();

	out << endl;
}