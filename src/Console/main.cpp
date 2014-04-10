#include <iostream>
#include <fstream>

#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/compiler/Compiler.hpp"

using namespace std;

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

	while (vm.GetStatus() == geogen::runtime::VIRTUAL_MACHINE_STATUS_READY)
	{		

		geogen::runtime::instructions::Instruction const* currentInstruction = vm.GetCallStack().Top().GetCodeBlockStack().Top().GetCurrentInstruction();
		cout << "Next: " << (currentInstruction->ToString()) << " on line " << currentInstruction->GetLocation().GetLine() << ", column " << currentInstruction->GetLocation().GetColumn() << ". Command? ";
        
        string line;
		getline(std::cin, line);
        
		size_t separatorPosition = line.find(" ");
		string command = line.substr(0, separatorPosition);
		string args = "";
		if (separatorPosition != string::npos)
		{
			args = line.substr(separatorPosition + 1);
		}        

		if (command == "s" || command == "step" || command == "")
		{
			cout << "Step" << std::endl;

			vm.Step();
		}
		else if (command == "t" || command == "stack")
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

		cout << "========================" << std::endl << std::endl;
	}
	
	//std::cout << script->GetSymbolNameTable().ToString();

	std::cout << "Virtual machine finished!";

	return 0;
}
