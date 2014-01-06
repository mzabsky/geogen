#include <iostream>
#include <fstream>

#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/compiler/Compiler.hpp"

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


	geogen::compiler::Compiler compiler;
	//HeightMap.Empty()        
	geogen::compiler::CompiledScript const* script = compiler.CompileScript(code.str());

	std::cout << script->GetRootCodeBlock().ToString();




	geogen::Number n(2);
	geogen::Number n2 = n / geogen::Number(3);
	//geogen::Number n5("3.14");

	std::cout << (double)n << " " << (double)n2 << " " << std::endl;

	return 0;
}
