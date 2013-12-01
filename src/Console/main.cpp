#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/compiler/Compiler.hpp"

int main(){
	geogen::Generator g(5);

	geogen::compiler::Compiler compiler;
	//HeightMap.Empty()        
	geogen::compiler::CompiledScript script = compiler.CompileScript(std::string("function aaa(x, y){}  yield HeightMap.Empty();"));

	return 0;
}
