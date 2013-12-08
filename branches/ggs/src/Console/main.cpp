#include <iostream>

#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/compiler/Compiler.hpp"

int main(){
	geogen::Generator g(5);

	geogen::compiler::Compiler compiler;
	//HeightMap.Empty()        
	geogen::compiler::CompiledScript script = compiler.CompileScript(std::string("\
metadata {\
StrItem: \"tralalal\",\
StrNumber: 5,\
StrSymbol: Symbol,\
KeyValSymbol: \
	{\
		SybSymbol: \"aaaa\"\
	}\
}\
function aaa(x, y){}  \
var a = 1 + 2;\
var b = true;\
var c = \"aaaa\";\
\
yield HeightMap.Empty();"));

	geogen::Number n(2);
	geogen::Number n2 = n / geogen::Number(3);
	//geogen::Number n5("3.14");

	std::cout << (double)n << " " << (double)n2 << " " << std::endl;

	return 0;
}
