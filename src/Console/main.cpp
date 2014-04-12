#include <iostream>
#include <fstream>

#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/compiler/Compiler.hpp"
#include <iomanip>

#include <windows.h>

using namespace std;

void SetConsoleColor(int foreground, int background)
{
	int finalcolor = (16 * background) + foreground;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), finalcolor);
}

int main(){
	geogen::Generator g(5);
	/*
	metadata {\
StrItem: \"tralalal\",\
StrNumber: 5,\
StrSymbol: Symbol,\
KeyValSymbol: \
	{\
		SybSymbol: \"aaaa\"\
	}\
}\
	*/
	
	std::stringstream  code;
	std::ifstream file("testinput.txt");
	std::string temp;
	while (std::getline(file, temp)) 
	{
		code << temp << std::endl;
	}

	std::string codeString = code.str();

	geogen::compiler::Compiler compiler;
	//HeightMap.Empty()        
	geogen::runtime::CompiledScript const* script = compiler.CompileScript(codeString);

	geogen::runtime::VirtualMachine vm(*script);
	//vm.Run();

	vector<string> codeLines;
	string currentLine;
	while (std::getline(code, currentLine, '\n'))
	{
		codeLines.push_back(currentLine);
	}

	unsigned numShowCodeLines = 5;
	while (vm.GetStatus() == geogen::runtime::VIRTUAL_MACHINE_STATUS_READY)
	{		
		geogen::runtime::instructions::Instruction const* currentInstruction = vm.GetCallStack().Top().GetCodeBlockStack().Top().GetCurrentInstruction();

		//system("cls");

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
					string currentLine = codeLines[currentLineNumber];

					cout
						<< std::setw(4)
						<< currentLineNumber << " "
						<< std::setw(4) << (currentLineNumber + 1 == currentInstruction->GetLocation().GetLine() ? ">>>" : "") << " ";

					if (currentLineNumber + 1 == currentInstruction->GetLocation().GetLine())
					{
						cout << currentLine.substr(0, currentInstruction->GetLocation().GetColumn());
						SetConsoleColor(7, 1);
						cout << currentLine[currentInstruction->GetLocation().GetColumn()];
						SetConsoleColor(7, 0);
						if (currentLine.length() > 0)
						{
							cout << currentLine.substr(currentInstruction->GetLocation().GetColumn() + 1);
						}
					}
					else
					{
						cout << currentLine;
					}
				}

				cout << std::endl;

				//	<< *it << std::endl;
			}
		}
		
		cout << "Next: " << (currentInstruction->ToString()) << " on line " << currentInstruction->GetLocation().GetLine() << ", column " << currentInstruction->GetLocation().GetColumn() << ". Command? ";
        
        string line;
		getline(std::cin, line);
        
		cout << "=====================================================" << std::endl << std::endl;

		size_t separatorPosition = line.find(" ");
		string command = line.substr(0, separatorPosition);
		string args = "";
		if (separatorPosition != string::npos)
		{
			args = line.substr(separatorPosition + 1);
		}        

		if (command == "step" || command == "")
		{
			cout << "Step" << std::endl;

			vm.Step();
		}
		else if (command == "s" || command == "stack")
        {
			cout << "Object stack:" << std::endl;

			cout << vm.GetObjectStack().ToString();
		}
		else if (command == "r" || command == "read")
		{
			cout << "Variable \"" << args << "\":" << std::endl;

			if (args == "")
			{
				cout << "Variable name not specified" << std::endl;
			}
			else
			{
				geogen::runtime::VariableTableItem* variableTableItem = vm.FindVariable(args);
				if (variableTableItem == NULL)
				{
					cout << "Undefined" << std::endl;
				}
				else
				{
					cout << "{ " << variableTableItem->GetValue()->ToString() << " } " << (variableTableItem->IsConst() ? "const" : "") << std::endl;
				}
			}
		}
		else if (command == "c" || command == "cs" || command == "callstack")
		{
			cout << "Call stack:" << std::endl;
			cout << vm.GetCallStack().ToString() << std::endl;
		}
		else if (command == "cbs" || command == "codeblockstack")
		{
			cout << "Code block stack:" << std::endl;
			cout << vm.GetCallStack().Top().GetCodeBlockStack().ToString() << std::endl;
		}
		else if (command == "cbc" || command == "codeblockcode")
		{
			geogen::runtime::CodeBlockStack& codeBlockStack = vm.GetCallStack().Top().GetCodeBlockStack();

			int codeBlockNumber = 0;
			if (args != "")
			{
				int codeBlockNumber = atoi(args.c_str());				
			}

			if (codeBlockNumber < codeBlockStack.Size())
			{
				cout << "Code block stack entry " << codeBlockNumber << ":" << std::endl;
				cout << (*(codeBlockStack.RBegin() - codeBlockNumber))->GetCodeBlock().ToString() << std::endl;
			}
			else
			{
				cout << "Incorrect code block stack entry number" << endl;
			}
		}
		else
		{
			cout << "Unknown command" << std::endl;
		}
	}
	
	//std::cout << script->GetSymbolNameTable().ToString();

	std::cout << "Virtual machine finished!";

	return 0;
}
