#include <iostream>

#include "Compiler.hpp"

#include "../antlr3/antlr3.h"

#include "../Grammar/output/GeoGenScriptLexer.h"
#include "../Grammar/output/GeoGenScriptParser.h"
#include "../Grammar/output/GeoGenScriptDecls.h"

#include "../runtime/instructions/IfInstruction.hpp"

using namespace std;
using namespace geogen;

CompiledScript::CompiledScript()
{
	this->metadata = NULL;
}

CompiledScript::~CompiledScript()
{
	if(this->metadata != NULL)
	{
		delete metadata;
	}
}
