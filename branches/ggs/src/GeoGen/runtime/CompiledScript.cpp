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
	for (std::vector<FunctionDefinition*>::iterator it = this->ownedFunctionDefinitions.begin(); it != this->ownedFunctionDefinitions.end(); it++)
	{
		delete *it;
	}

	for (std::vector<TypeDefinition*>::iterator it = this->ownedTypeDefinitions.begin(); it != this->ownedTypeDefinitions.end(); it++)
	{
		delete *it;
	}

	if(this->metadata != NULL)
	{
		delete metadata;
	}
}

bool CompiledScript::AddGlobalFunctionDefinition(FunctionDefinition* functionDefintion)
{
	if (!this->globalFunctionDefinitions.AddItem(functionDefintion, functionDefintion->GetName()))
	{
		return false;
	}

	this->ownedFunctionDefinitions.push_back(functionDefintion);

	return true;
}

bool CompiledScript::AddTypeDefinition(TypeDefinition* typeDefinition)
{
	if (!this->typeDefinitions.AddItem(typeDefinition, typeDefinition->GetName()))
	{
		return false;
	}

	this->ownedTypeDefinitions.push_back(typeDefinition);

	return true;
}

void CompiledScript::AddLibrary(Library const* library)
{
	for (SymbolDefinitionTable<TypeDefinition>::const_iterator it = library->GetTypeDefinitions().Begin(); it != library->GetTypeDefinitions().Begin(); it++)
	{
		if (!this->typeDefinitions.AddItem(it->second, it->first))
		{
			throw ApiUsageException("Type with the same name is already registered.");
		}
	}

	for (SymbolDefinitionTable<FunctionDefinition>::const_iterator it = library->GetGlobalFunctionDefinitions().Begin(); it != library->GetGlobalFunctionDefinitions().Begin(); it++)
	{
		if (this->globalFunctionDefinitions.AddItem(it->second, it->first))
		{
			throw ApiUsageException("Global function with the same name is already registered.");
		}
	}

	for (SymbolDefinitionTable<VariableDefinition>::const_iterator it = library->GetGlobalVariableDefinitions().Begin(); it != library->GetGlobalVariableDefinitions().Begin(); it++)
	{
		if (this->globalVariableDefinitions.AddItem(it->second, it->first))
		{
			throw ApiUsageException("Global variable with the same name is already registered.");
		}
	}
}