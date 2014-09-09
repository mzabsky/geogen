#include "../antlr3/antlr3.h"

#include "../InternalErrorException.hpp"

#include "../Grammar/output/GeoGenScriptLexer.h"
#include "../Grammar/output/GeoGenScriptParser.h"
#include "../Grammar/output/GeoGenScriptDecls.h"

#include "../runtime/instructions/IfInstruction.hpp"
#include "../runtime/ScriptFunctionDefinition.hpp"

#include "Compiler.hpp"
#include "AntlrRaiiWrappers.hpp"
#include "../corelib/ParametersTypeDefinition.hpp"
#include "../utils/StringUtils.hpp"
#include "MainMapNotSupportedByScriptException.hpp"


using namespace std;
using namespace geogen;
using namespace geogen::runtime;
using namespace geogen::utils;
//using namespace geogen_generated;

Compiler::Compiler(Configuration configuration) : configuration(configuration) {}

CompiledScript* Compiler::CompileScript(String const& code) const
{
	auto_ptr<CompiledScript> script(new CompiledScript(code));
	auto_ptr<CodeBlock> rootCodeBlock(new CodeBlock());

	script->SetConfiguration(this->configuration);

#ifdef GEOGEN_WCHAR
	AnlrInputStreamWrapper input((pANTLR3_UINT8)code.c_str(), ANTLR3_ENC_UTF16LE, code.length() * 2, (pANTLR3_UINT8)"");
#else
	AnlrInputStreamWrapper input((pANTLR3_UINT8)code.c_str(), ANTLR3_ENC_8BIT, code.length(), (pANTLR3_UINT8)"");
#endif

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

		if (this->configuration.MainMapIsMandatory)
		{
			if (find(script->GetSupportedMaps().begin(), script->GetSupportedMaps().end(), renderer::Renderer::MAP_NAME_MAIN) == script->GetSupportedMaps().end())
			{
				throw MainMapNotSupportedByScriptException(CodeLocation(walker.GetPtr()->lines.size(), 0));
			}
		}
	}

	return script.release();
}