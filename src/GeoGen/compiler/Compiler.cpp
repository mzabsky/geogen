#include <iostream>

#include "Compiler.hpp"

#include "../antlr3/antlr3.h"

#include "../Grammar/output/GeoGenScriptLexer.h"
#include "../Grammar/output/GeoGenScriptParser.h"
#include "../Grammar/output/GeoGenScriptDecls.h"

using namespace std;
using namespace geogen;
using namespace geogen::compiler;
//using namespace geogen_generated;

Compiler::Compiler(){}

const CompiledScript Compiler::CompileScript(std::string& code) const
{
	/*GeoGenScriptLexer::InputStreamType input((const unsigned char*)code.c_str(), ANTLR_ENC_8BIT, code.length(), NULL);
	GeoGenScriptLexer lxr(&input);

	GeoGenScriptParser::TokenStreamType tstream(ANTLR_SIZE_HINT, lxr.get_tokSource() );
	GeoGenScriptParser psr(&tstream);

	psr.script();*/

	CompiledScript* script = new CompiledScript();

	pANTLR3_INPUT_STREAM input;
	pGeoGenScriptLexer lex;
	pANTLR3_COMMON_TOKEN_STREAM tokens;
	pGeoGenScriptParser parser;

	input = antlr3StringStreamNew((pANTLR3_UINT8)code.c_str(), ANTLR3_ENC_8BIT, code.length(), (pANTLR3_UINT8)"");
	lex = GeoGenScriptLexerNew(input);	
	tokens = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lex));
	//pANTLR3_LIST list = antlr3ListNew(5);
	//tokens->getTokensList(tokens, 0, 1, list);
	parser = GeoGenScriptParserNew(tokens);

	GeoGenScriptParser_script_return r = parser->script(parser);

	if (parser->pParser->rec->state->errorCount > 0)
    {
		fprintf(stderr, "The parser returned %d errors, tree walking aborted.\n", parser->pParser->rec->state->errorCount);
    }
    else
	{
	    pANTLR3_BASE_TREE tree = r.tree;
	
		pANTLR3_COMMON_TREE_NODE_STREAM	nodes = antlr3CommonTreeNodeStreamNewTree(tree, ANTLR3_SIZE_HINT);;
		pGeoGenScriptDecls walker = GeoGenScriptDeclsNew(nodes);
		walker->compiledScript = script;		

		printf("Tree : %s\n", tree->toStringTree(tree)->chars);

		walker->script(walker);

		nodes->free(nodes);

		FunctionDefinition* def = script->GetFunctionDefinitions()->GetItem("aaa");

		walker->free(walker);

		for(SymbolDefinitionTable<FunctionDefinition>::const_iterator i = script->GetFunctionDefinitions()->Begin(); i != script->GetFunctionDefinitions()->End(); i++){
			FunctionDefinition const*  d = i->second;
			std::cout << i->second->GetName() << std::endl;
		}

		for(SymbolNameTable::const_iterator i = script->GetSymbolNameTable()->Begin(); i != script->GetSymbolNameTable()->End(); i++){
			std::cout << *i << std::endl;
		}
	}

	parser->free(parser);
	tokens->free(tokens);
	lex->free(lex);
	input->close(input);

	return CompiledScript();
}