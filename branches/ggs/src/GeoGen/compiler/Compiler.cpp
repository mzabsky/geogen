#include <iostream>

#include "../antlr3/antlr3.h"

#include "../InternalErrorException.hpp"

#include "../Grammar/output/GeoGenScriptLexer.h"
#include "../Grammar/output/GeoGenScriptParser.h"
#include "../Grammar/output/GeoGenScriptDecls.h"

#include "../runtime/instructions/IfInstruction.hpp"
#include "../runtime/ScriptFunctionDefinition.hpp"

#include "Compiler.hpp"
#include "AntlrRaiiWrappers.hpp"



using namespace std;
using namespace geogen;
using namespace geogen::runtime;
//using namespace geogen_generated;

Compiler::Compiler(){}

CompiledScript* Compiler::CompileScript(std::string const& code) const
{
	auto_ptr<CompiledScript> script(new CompiledScript());
	auto_ptr<CodeBlock> rootCodeBlock(new CodeBlock());

	AnlrInputStreamWrapper input ((pANTLR3_UINT8)code.c_str(), ANTLR3_ENC_8BIT, code.length(), (pANTLR3_UINT8)"");
	AntlrLexerWrapper lex(input);	
	AntlrTokenStreamWrapper tokens(ANTLR3_SIZE_HINT, lex);
	AntlrParserWrapper parser(tokens);

	GeoGenScriptParser_script_return r = parser.GetPtr()->script(parser.GetPtr());

	if (parser.GetPtr()->pParser->rec->state->errorCount > 0)
    {
		// This error should already have caused an exception in the parser.
		throw InternalErrorException("Unreported parser error.");
    }
    else
	{
		AntlrTreeNodeStreamWrapper nodes(r.tree, ANTLR3_SIZE_HINT);
		AntlrVectorFactoryWrapper vectorFactory;
		AntlrTreeWalkerWrapper walker(nodes);
		
		walker.GetPtr()->compiledScript = script.get();		
		walker.GetPtr()->vectors = vectorFactory.GetPtr();
		walker.GetPtr()->rootCodeBlock = rootCodeBlock.get();

		//printf("Tree : %s\n", r.tree->toString(r.tree)->chars);

		//printf("Tree : %s\n", r.tree->toStringTree(r.tree)->chars);

		walker.GetPtr()->script(walker.GetPtr());

		if (walker.GetPtr()->pTreeParser->rec->state->errorCount > 0)
		{
			// This error should already have caused an exception in the tree walker.
			throw InternalErrorException("Unreported tree walker error.");
		}

		if (script->GetMetadata() != NULL)
		{
			script->SetMetadata(new MetadataKeyValueCollection());
		}

		ScriptFunctionDefinition* mainFunctionDefinition = new ScriptFunctionDefinition(CompiledScript::MAIN_FUNCTION_NAME, CodeLocation(0, 0), 0);
		mainFunctionDefinition->GetRootCodeBlock().AddInstruction(new instructions::YieldAsMainInstruction(CodeLocation(0, 0)));
		mainFunctionDefinition->GetRootCodeBlock().MoveInstructionsFrom(*walker.GetPtr()->rootCodeBlock);
		if (!script->AddGlobalFunctionDefinition(mainFunctionDefinition))
		{
			delete mainFunctionDefinition;
			throw InternalErrorException("Main function name conflict.");
		}

		//ScriptFunctionDefinition* code2 = ((ScriptFunctionDefinition*)script->GetGlobalFunctionDefinitions().GetItem("aaa"));

		/*cout << "=======================" << endl;

		std::string code = script->GetRootCodeBlock().ToString();

		cout << code << endl;
		cout << "=======================" << endl;

		

		cout << code2 << endl;
		cout << "=======================" << endl;

		std::string code3 = script->GetSymbolNameTable().ToString();

		cout << code3 << endl;
		cout << "=======================" << endl;

		for(SymbolDefinitionTable<FunctionDefinition>::const_iterator i = script->GetGlobalFunctionDefinitions().Begin(); i != script->GetGlobalFunctionDefinitions().End(); i++){
			ScriptFunctionDefinition const*  d = (ScriptFunctionDefinition const*)i->second;
			std::cout << i->second->GetName() << " " << ((ScriptFunctionDefinition*)i->second)->GetParameterCount() << std::endl;
		}

		for(SymbolNameTable::const_iterator i = script->GetSymbolNameTable().Begin(); i != script->GetSymbolNameTable().End(); i++){
			std::cout << *i << std::endl;
		}*/
	}

	return script.release();
}