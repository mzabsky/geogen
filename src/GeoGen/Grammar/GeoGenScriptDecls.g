tree grammar GeoGenScriptDecls;

options 
{
    tokenVocab	    = GeoGenScript;
    language	    = C;
    ASTLabelType	= pANTLR3_BASE_TREE;
}

/*@includes
{
	#include "../../Compiler/compiler.hpp"
}

@members 
{
	geogen::compiler::Compiler* compiler;
}*/

enumDecl: ^(ENUM enumValue*);

enumValue: ^(IDENTIFIER expression);

expression
	:	IDENTIFIER;