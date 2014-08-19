#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/compiler/Compiler.hpp"
#include <iomanip>
#include <fstream>

#include <windows.h>
#include "..\GeoGen\utils\StringUtils.hpp"

#include "CommandTable.hpp"

#include "Loader.hpp"
#include "ConsoleUtils.hpp"
#include "ProgramArguments.hpp"
#include "ArgDesc.hpp"

using namespace geogen;
using namespace console;
using namespace compiler;
using namespace runtime;
using namespace random;
using namespace std;

int main(int argc, char** argv){
	ProgramArguments programArguments;

	// initialize argument support
	ArgDesc args(argc, argv);
	//args.SetPosArgsVector(_params.script_args);

	args.AddStringArg(GG_STR('i'), GG_STR("input"), GG_STR("Input script to be executed."), GG_STR("FILE"), &programArguments.inputFile);
	args.AddStringArg(GG_STR('o'), GG_STR("output"), GG_STR("Output file, the extension determines file type of the output (*.bmp for Windows Bitmap, *.shd for GeoGen Short Height Data and *.pgm for Portable Gray Map are allowed). Set to \"../temp/out.bmp\" by default."), GG_STR("FILE"), &programArguments.outputDirectory);
	//args.AddStringArg(GG_STR('d'), GG_STR("output-directory"), GG_STR("Directory where secondary maps will be saved. Set to \"../temp/\" by default."), GG_STR("DIRECTORY"), &_params.output_directory);
	//args.AddStringArg(GG_STR('v'), GG_STR("overlay"), GG_STR("Overlay file to be mapped on the output. This file must be a Windows Bitmap file one pixel high and either 256 or 511 pixels wide."), GG_STR("FILE"), &_params.overlay_file);

	args.AddIntArg(GG_STR('s'), GG_STR("seed"), GG_STR("Pseudo-random generator seed. Maps generated with same seed, map script, arguments and generator version are always the same."), GG_STR("SEED"), &*(int*)(programArguments.seed));

	//args.AddBoolArg(GG_STR('a'), GG_STR("all-random"), GG_STR("All unset script arguments are generated randomly."), &_params.all_random);
	//args.AddBoolArg(GG_STR('z'), GG_STR("ignore-zero"), GG_STR("Height data range will be rescaled to fit the output file format including negative value. Zero level will probably not be preserved. Allows to fit negative values into format, which doesn't support them (Windows Bitmap)."), &_params.ignore_zero);
	//args.AddBoolArg(GG_STR('n'), GG_STR("no-rescaling"), GG_STR("The height data will not be rescaled at all. Might cause color overflows if the format's value range is lower than <-32787, 32787>."), &_params.no_rescaling);
	args.AddBoolArg(GG_STR('?'), GG_STR("help"), GG_STR("Displays this help."), &programArguments.displayHelp);
	//args.AddBoolArg(GG_STR('x'), GG_STR("syntax-check"), GG_STR("Will print OKAY if script is compilable or descibe the error found."), &_params.syntax_check_mode);
	//args.AddBoolArg(GG_STR('e'), GG_STR("simple"), GG_STR("Mode which allows all necessary data to be entered interactively. This mode is automatically activaded if no params were entered."), &_params.stupid_mode);
	//args.AddBoolArg(GG_STR('m'), GG_STR("manual"), GG_STR("Script arguments will be entered interactively."), &_params.manual_mode);
	//args.AddBoolArg(GG_STR('D'), GG_STR("disable-secondary-maps"), GG_STR("All secondary maps will be immediately discarded, ReturnAs calls will be effectively skipped."), &_params.disable_secondary_maps);
	//args.AddBoolArg(GG_STR('V'), GG_STR("overlay-as-copy"), GG_STR("Color files with overlays will be saved as copies."), &_params.overlay_as_copy);
	//args.AddIntArg(GG_STR('g'), GG_STR("grid"), GG_STR("Renders a grid onto the overlay file."), GG_STR("SIZE"), &_params.grid_size);
	//args.AddBoolArg(GG_STR('h'), GG_STR("split-range"), GG_STR("Splits the value range of a file format, which doesn't support negative values, so lower half of the range covers negaive values and upper half covers positive values. Value \"(max + 1) / 2\" will be treated as zero."), &_params.split_range);

	args.Scan();

	if (programArguments.displayHelp)
	{
		args.PrintHelpString();
		return 0;
	}

	Loader loader(Cin, Cout, programArguments);
	loader.Run();

	return 0;

	/*StringStream  code;
	IFStream file("testinput.txt");
	String temp;
	while (std::getline<Char>(file, temp)) 
	{
		code << temp << std::endl;
	}

	String codeString = code.str();
	
	String dumpString = GG_STR("");
	try
	{
		Compiler compiler;
		CompiledScript const* script = compiler.CompileScript(codeString);

		vector<String> codeLines = geogen::utils::StringToLines(codeString);

		String input = "";
		unsigned numShowCodeLines = 5;

		Debugger debugger(Cin, Cout, *script, geogen::runtime::ScriptParameters());

		try
		{
			debugger.Run();
		}		
		catch (RuntimeException& e)
		{
			dumpString = debugger.GetVirtualMachine()->ToString();
			throw;
		}

		Cout << "Virtual machine finished!";
	}
	catch (GeoGenException& e)
	{
		HighlightRed();
		Cout << "Error GGE" << e.GetErrorCode() << ": " << e.GetDetailMessage() << endl;
		Unhighlight();

		if (dumpString != GG_STR(""))
		{
			Cout << GG_STR("Do you wish to save dump file? (y/n, default = n) ");
			String saveDumpResponse;
			getline(Cin, saveDumpResponse);

			if (saveDumpResponse == GG_STR("y") || saveDumpResponse == GG_STR("Y") || saveDumpResponse == GG_STR("yes"))
			{
				Cout << GG_STR("Dump file name (default = dump.txt): ");
				String dumpFileName;
				getline(Cin, dumpFileName);
				
				if (dumpFileName == GG_STR(""))
				{
					dumpFileName = "dump.txt";
				}

				OFStream dumpStream(dumpFileName);
				dumpStream << dumpString;
				dumpStream.flush();

				if (dumpStream.fail())
				{
					Cout << GG_STR("Could not write dump file.");
				}
				else 
				{
					Cout << GG_STR("Saved dump file \"") << dumpFileName << GG_STR("\".") << endl;
				}
			}
			else 
			{
				Cout << GG_STR("Discarded dump file.");
			}
		}
	}
	*/
	/*
	if (false)
	while (vm.GetStatus() == geogen::runtime::VIRTUAL_MACHINE_STATUS_READY)
	{		
		geogen::runtime::instructions::Instruction const* currentInstruction = vm.GetCallStack().Top().GetCodeBlockStack().Top().GetCurrentInstruction();

		system("cls");

		

		Cout << endl;

		{
			Cout << "Input: " << input << endl << endl;

			size_t separatorPosition = input.find(" ");
			string command = input.substr(0, separatorPosition);
			string args = "";
			if (separatorPosition != String::npos)
			{
				args = input.substr(separatorPosition + 1);
			}

			if (command == "step" || command == "")
			{
				Cout << "Step" << std::endl;

				vm.Step();
			}
			else if (command == "h" || command == "?" || command == "help")
			{
				Cout << "h - Help" << std::endl;
				Cout << "step - Step (or use Enter)" << std::endl;
				Cout << "s - Object stack" << std::endl;
				Cout << "r - Get a value of a variable" << std::endl;
				Cout << "c - Call stack" << std::endl;
				Cout << "cbs - Code block stack" << std::endl;
				Cout << "cbc [x = 0] - Code of x-th topmost code block." << std::endl;
				Cout << "o - Managed objects" << std::endl;
			}
			else if (command == "s" || command == "stack")
			{
				Cout << "Object stack:" << std::endl;

				Cout << vm.GetObjectStack().ToString();
			}
			else if (command == "r" || command == "read")
			{
				Cout << "Variable \"" << args << "\":" << std::endl;

				if (args == "")
				{
					Cout << "Variable name not specified" << std::endl;
				}
				else
				{
					geogen::runtime::VariableTableItem* variableTableItem = vm.FindVariable(AnyStringToString(args));
					if (variableTableItem == NULL)
					{
						Cout << "Undefined" << std::endl;
					}
					else
					{
						Cout << "{ " << variableTableItem->GetValue()->ToString() << " } " << (variableTableItem->IsConst() ? "const" : "") << std::endl;
					}
				}
			}
			else if (command == "c" || command == "cs" || command == "callstack")
			{
				Cout << "Call stack:" << std::endl;
				Cout << vm.GetCallStack().ToString() << std::endl;
			}
			else if (command == "cbs" || command == "codeblockstack")
			{
				Cout << "Code block stack:" << std::endl;
				Cout << vm.GetCallStack().Top().GetCodeBlockStack().ToString() << std::endl;
			}
			else if (command == "cbc" || command == "codeblockcode")
			{
				geogen::runtime::CodeBlockStack& codeBlockStack = vm.GetCallStack().Top().GetCodeBlockStack();

				unsigned codeBlockNumber = 0;
				if (args != "")
				{
					int codeBlockNumber = atoi(args.c_str());
				}

				if (codeBlockNumber < codeBlockStack.Size())
				{
					Cout << "Code block stack entry " << codeBlockNumber << ":" << std::endl;
					Cout << (*(codeBlockStack.RBegin() - codeBlockNumber))->GetCodeBlock().ToString() << std::endl;
				}
				else
				{
					Cout << "Incorrect code block stack entry number" << endl;
				}
			}
			else if (command == "o" || command == "mm" || command == "managedobjects")
			{
				Cout << "Managed objects:" << std::endl;
				Cout << vm.GetMemoryManager().ToString() << std::endl;
			}
			else
			{
				Cout << "Unknown command" << std::endl;
			}
		}

		Cout << endl << "Next instruction: " << (currentInstruction->ToString()) << " on line " << currentInstruction->GetLocation().GetLine() << ", column " << currentInstruction->GetLocation().GetColumn() << ". " << endl << endl << "Command? ";
        
		getline<Char>(Cin, input);
	}*/
	
	//std::cout << script->GetSymbolNameTable().ToString();	

	return 0;
}
