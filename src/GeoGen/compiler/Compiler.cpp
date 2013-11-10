#include "..\Grammar\output\GeoGenScriptLexer.hpp"
#include "..\Grammar\output\GeoGenScriptParser.hpp"

#include "Compiler.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::compiler;
using namespace geogen_generated;

CompiledScript Compiler::CompileScript(std::string& code)
{
	GeoGenScriptLexer::InputStreamType input((const unsigned char*)code.c_str(), ANTLR_ENC_8BIT, code.length(), NULL);
	GeoGenScriptLexer lxr(&input);

	GeoGenScriptParser::TokenStreamType tstream(ANTLR_SIZE_HINT, lxr.get_tokSource() );
	GeoGenScriptParser psr(&tstream);

	psr.script();

	return CompiledScript();
}