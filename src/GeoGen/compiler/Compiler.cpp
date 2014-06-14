#include "../antlr3/antlr3.h"

#include "../InternalErrorException.hpp"

#include "../Grammar/output/GeoGenScriptLexer.h"
#include "../Grammar/output/GeoGenScriptParser.h"
#include "../Grammar/output/GeoGenScriptDecls.h"

#include "../runtime/instructions/IfInstruction.hpp"
#include "../runtime/ScriptFunctionDefinition.hpp"

#include "Compiler.hpp"
#include "AntlrRaiiWrappers.hpp"
#include "..\corelib\ParametersTypeDefinition.hpp"
#include "..\utils\StringUtils.hpp"


using namespace std;
using namespace geogen;
using namespace geogen::runtime;
using namespace geogen::utils;
//using namespace geogen_generated;

Compiler::Compiler(){}

CompiledScript* Compiler::CompileScript(String const& code) const
{
	auto_ptr<CompiledScript> script(new CompiledScript());
	auto_ptr<CodeBlock> rootCodeBlock(new CodeBlock());

	wstring asciiCode = StringToWstring(code);
	AnlrInputStreamWrapper input((pANTLR3_UINT8)asciiCode.c_str(), ANTLR3_ENC_UTF16LE, code.length() * 2, (pANTLR3_UINT8)"");
	AntlrLexerWrapper lex(input);	
	AntlrTokenStreamWrapper tokens(ANTLR3_SIZE_HINT, lex);
	AntlrParserWrapper parser(tokens);

	GeoGenScriptParser_script_return r = parser.GetPtr()->script(parser.GetPtr());

	if (parser.GetPtr()->pParser->rec->state->errorCount > 0)
    {
		// This error should already have caused an exception in the parser.
		throw InternalErrorException(GG_STR("Unreported parser error."));
    }
    else
	{
		AntlrTreeNodeStreamWrapper nodes(r.tree, ANTLR3_SIZE_HINT);
		AntlrVectorFactoryWrapper vectorFactory;
		AntlrTreeWalkerWrapper walker(nodes);
		
		walker.GetPtr()->compiledScript = script.get();		
		walker.GetPtr()->vectors = vectorFactory.GetPtr();
		walker.GetPtr()->rootCodeBlock = rootCodeBlock.get();
		walker.GetPtr()->lines = StringToLines(code);

		//printf("Tree : %s\n", r.tree->toString(r.tree)->chars);

		//printf("Tree : %s\n", r.tree->toStringTree(r.tree)->chars);

		walker.GetPtr()->script(walker.GetPtr());

		if (walker.GetPtr()->pTreeParser->rec->state->errorCount > 0)
		{
			// This error should already have caused an exception in the tree walker.
			throw InternalErrorException(GG_STR("Unreported tree walker error."));
		}

		/*if (script->GetMetadata() == NULL)
		{
			script->SetMetadata(new MetadataKeyValueCollection());
		}*/

		ScriptFunctionDefinition* mainFunctionDefinition = new ScriptFunctionDefinition(CompiledScript::MAIN_FUNCTION_NAME, CodeLocation(0, 0), 0);
		//mainFunctionDefinition->GetRootCodeBlock().AddInstruction(new instructions::YieldAsMainInstruction(CodeLocation(1,1)));
		mainFunctionDefinition->GetRootCodeBlock().MoveInstructionsFrom(*walker.GetPtr()->rootCodeBlock);
		if (!script->AddGlobalFunctionDefinition(mainFunctionDefinition))
		{
			delete mainFunctionDefinition;
			throw InternalErrorException(GG_STR("Main function name conflict."));
		}

		{
			ScriptParameters scriptParameters = script->CreateScriptParameters();
			
			ParametersTypeDefinition* parametersTypeDefinition = new ParametersTypeDefinition(scriptParameters);
			if (!script->AddTypeDefinition(parametersTypeDefinition))
			{
				delete parametersTypeDefinition;
				throw InternalErrorException(GG_STR("Parameters object name conflict."));
			}
		}
	}

	return script.release();
}