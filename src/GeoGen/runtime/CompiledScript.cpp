#include <iostream>

#include "CompiledScript.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

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
