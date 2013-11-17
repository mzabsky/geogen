#include "Compiler.hpp"

#include "../Grammar/output/GeoGenScriptLexer.h"
#include "../Grammar/output/GeoGenScriptParser.h"

#include "../antlr3/antlr3.h"

using namespace std;
using namespace geogen;
using namespace geogen::compiler;

CompiledScript Compiler::CompileScript(std::string& code)
{
	pANTLR3_INPUT_STREAM input;
	pGeoGenScriptLexer lex;
	pANTLR3_COMMON_TOKEN_STREAM tokens;
	pGeoGenScriptParser parser;

	input = antlr3StringStreamNew((unsigned char*)code.c_str(), ANTLR3_ENC_8BIT, code.length(), NULL);
	lex = GeoGenScriptLexerNew(input);
	tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
	parser = GeoGenScriptParserNew(tokens);

	GeoGenScriptParser_script_return r = parser->script(parser);

	pANTLR3_BASE_TREE tree = r.tree;

	/*ExprTreeEvaluator eval;
	int rr = eval.run(tree);
	cout << "Evaluator result: " << rr << '\n';*/

	parser->free(parser);
	tokens->free(tokens);
	lex->free(lex);
	input->close(input);


	return CompiledScript();
}