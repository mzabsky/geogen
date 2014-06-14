#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/compiler/Compiler.hpp"
#include <iomanip>
#include <fstream>

#include <windows.h>
#include "..\GeoGen\utils\StringUtils.hpp"

using namespace geogen;
using namespace std;

void SetConsoleColor(int foreground, int background)
{
	int finalcolor = (16 * background) + foreground;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), finalcolor);
}

int main(){
	StringStream  code;
	IFStream file("testinput.txt");
	String temp;
	while (std::getline<Char>(file, temp)) 
	{
		code << temp << std::endl;
	}

	String codeString = code.str();

	geogen::compiler::Compiler compiler;
	//HeightMap.Empty()        
	geogen::runtime::CompiledScript const* script = compiler.CompileScript(codeString);

	geogen::runtime::VirtualMachine vm(*script, geogen::runtime::ScriptParameters());
	//vm.Run();

	vector<String> codeLines = geogen::utils::StringToLines(codeString);

	String input = "";
	unsigned numShowCodeLines = 5;
	while (vm.GetStatus() == geogen::runtime::VIRTUAL_MACHINE_STATUS_READY)
	{		
		geogen::runtime::instructions::Instruction const* currentInstruction = vm.GetCallStack().Top().GetCodeBlockStack().Top().GetCurrentInstruction();

		system("cls");

		if (numShowCodeLines > 0)
		{//codeLines.at(max(currentInstruction->GetLocation().GetLine(), 0)
			for (
				int i = -(int)numShowCodeLines;
				i < (int)numShowCodeLines;
				i++)
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
						SetConsoleColor(7, 1);
						Cout << currentLine[currentInstruction->GetLocation().GetColumn()];
						SetConsoleColor(7, 0);
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
	}
	
	//std::cout << script->GetSymbolNameTable().ToString();

	Cout << "Virtual machine finished!";

	return 0;
}
