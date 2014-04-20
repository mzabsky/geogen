#include <iostream>

#include "CompiledScript.hpp"
#include "Library.hpp"
#include "FunctionDefinition.hpp"
#include "VariableDefinition.hpp"
#include "TypeDefinition.hpp"
#include "..\compiler\ParametersNotKeyValueSectionException.hpp"
#include "..\compiler\IncorrectParameterDefinitionException.hpp"
#include "NumberScriptParameter.hpp"
#include "MetadataString.hpp"
#include "MetadataIdentifier.hpp"
#include "..\InternalErrorException.hpp"
#include "MetadataBoolean.hpp"
#include "BooleanScriptParameter.hpp"
#include "MetadataNumber.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

const string CompiledScript::MAIN_FUNCTION_NAME = "<main>";

CompiledScript::CompiledScript() : metadata(CodeLocation(0, 0))
{
	this->AddLibrary(&this->coreLibrary);
//	this->metadata = NULL;
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

/*	if(this->metadata != NULL)
	{
		delete metadata;
	}*?*/
}

bool CompiledScript::AddGlobalFunctionDefinition(FunctionDefinition* functionDefintion)
{
	if (!this->globalFunctionDefinitions.AddItem(functionDefintion))
	{
		return false;
	}

	this->ownedFunctionDefinitions.push_back(functionDefintion);

	return true;
}

bool CompiledScript::AddTypeDefinition(TypeDefinition* typeDefinition)
{
	if (!this->typeDefinitions.AddItem(typeDefinition))
	{
		return false;
	}

	this->ownedTypeDefinitions.push_back(typeDefinition);

	return true;
}

void CompiledScript::AddLibrary(Library const* library)
{
	for (SymbolDefinitionTable<TypeDefinition>::const_iterator it = library->GetTypeDefinitions().Begin(); it != library->GetTypeDefinitions().End(); it++)
	{
		if (!this->typeDefinitions.AddItem(it->second))
		{
			throw ApiUsageException("Type with the same name is already registered.");
		}
	}

	for (SymbolDefinitionTable<FunctionDefinition>::const_iterator it = library->GetGlobalFunctionDefinitions().Begin(); it != library->GetGlobalFunctionDefinitions().End(); it++)
	{
		if (!this->globalFunctionDefinitions.AddItem(it->second))
		{
			throw ApiUsageException("Global function with the same name is already registered.");
		}
	}

	for (SymbolDefinitionTable<VariableDefinition>::const_iterator it = library->GetGlobalVariableDefinitions().Begin(); it != library->GetGlobalVariableDefinitions().End(); it++)
	{
		if (!this->globalVariableDefinitions.AddItem(it->second))
		{
			throw ApiUsageException("Global variable with the same name is already registered.");
		}
	}
}


void CompiledScript::CreateScriptParameters(ScriptParameters& table) const
{
	// This code is first invoked by the compiler, so it throws compiler exceptions.

	if (!this->GetMetadata().ContainsItem("Parameters"))
	{
		return;
	}

	if (this->GetMetadata().GetItem("Parameters")->GetType() != METADATA_TYPE_KEYVALUE_COLLECTION)
	{
		throw compiler::ParametersNotKeyValueSectionException(this->GetMetadata().GetItem("Parameters")->GetLocation());
	}

	MetadataKeyValueCollection const* parametersSection = dynamic_cast<MetadataKeyValueCollection const*>(this->GetMetadata().GetItem("Parameters"));

	for (MetadataKeyValueCollection::const_iterator it = parametersSection->Begin(); it != parametersSection->End(); it++)
	{		
		string name = it->first;
		CodeLocation location = it->second->GetLocation();
		
		if (it->second->GetType() != METADATA_TYPE_KEYVALUE_COLLECTION)
		{
			throw compiler::IncorrectParameterDefinitionException(location, name);
		}

		MetadataKeyValueCollection const* currentSection = dynamic_cast<MetadataKeyValueCollection const*>(it->second);

		string label;
		if (currentSection->ContainsItem("Label"))
		{
			if (currentSection->GetItem("Label")->GetType() != METADATA_TYPE_STRING)
			{
				throw compiler::IncorrectParameterDefinitionException(location, name);
			}

			label = dynamic_cast<MetadataString const*>(currentSection->GetItem("Label"))->GetValue();
		}
		else
		{
			label = name;
		}
		
		string description;
		if (currentSection->ContainsItem("Description"))
		{
			if (currentSection->GetItem("Description")->GetType() != METADATA_TYPE_STRING)
			{
				throw compiler::IncorrectParameterDefinitionException(location, name);
			}

			description = dynamic_cast<MetadataString const*>(currentSection->GetItem("Description"))->GetValue();
		}

		ScriptParameterType parameterType = SCRIPT_PARAMETER_TYPE_NUMBER;
		if (currentSection->ContainsItem("Type"))
		{
			if (currentSection->GetItem("Type")->GetType() != METADATA_TYPE_IDENTIFIER)
			{
				throw compiler::IncorrectParameterDefinitionException(location, name);
			}

			string parameterTypeName = dynamic_cast<MetadataIdentifier const*>(currentSection->GetItem("Type"))->GetValue();
			if (parameterTypeName == "Boolean")
			{
				parameterType = SCRIPT_PARAMETER_TYPE_BOOLEAN;
			}
			else if (parameterTypeName == "Number")
			{
				parameterType = SCRIPT_PARAMETER_TYPE_NUMBER;
			}
			else
			{
				throw compiler::IncorrectParameterDefinitionException(location, name);
			}
		}

		ScriptParameter* parameter = NULL;
		switch (parameterType)
		{		
		case SCRIPT_PARAMETER_TYPE_BOOLEAN:
			{
				bool defaultValue = false;
				if (currentSection->ContainsItem("Default"))
				{
					if (currentSection->GetItem("Default")->GetType() != METADATA_TYPE_BOOLEAN)
					{
						throw compiler::IncorrectParameterDefinitionException(location, name);
					}

					defaultValue = dynamic_cast<MetadataBoolean const*>(currentSection->GetItem("Default"))->GetValue();			
				}

				parameter = new BooleanScriptParameter(name, label, description, defaultValue);
				break;
			}
		case SCRIPT_PARAMETER_TYPE_NUMBER:
			{
				Number defaultValue = 0;
				if (currentSection->ContainsItem("Default"))
				{
					if (currentSection->GetItem("Default")->GetType() != METADATA_TYPE_NUMBER)
					{
						throw compiler::IncorrectParameterDefinitionException(location, name);
					}

					defaultValue = dynamic_cast<MetadataNumber const*>(currentSection->GetItem("Default"))->GetValue();
				}

				Number min = 0;
				if (currentSection->ContainsItem("Min"))
				{
					if (currentSection->GetItem("Min")->GetType() != METADATA_TYPE_NUMBER)
					{
						throw compiler::IncorrectParameterDefinitionException(location, name);
					}

					min = dynamic_cast<MetadataNumber const*>(currentSection->GetItem("Min"))->GetValue();
				}

				Number max = 100;
				if (currentSection->ContainsItem("Max"))
				{
					if (currentSection->GetItem("Max")->GetType() != METADATA_TYPE_NUMBER)
					{
						throw compiler::IncorrectParameterDefinitionException(location, name);
					}

					max = dynamic_cast<MetadataNumber const*>(currentSection->GetItem("Max"))->GetValue();
				}

				if (defaultValue > max || defaultValue < min)
				{
					throw compiler::IncorrectParameterDefinitionException(location, name);
				}

				ScriptParameterValueRestriction restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED;
				if (currentSection->ContainsItem("Restriction"))
				{
					if (currentSection->GetItem("Restriction")->GetType() != METADATA_TYPE_IDENTIFIER)
					{
						throw compiler::IncorrectParameterDefinitionException(location, name);
					}

					string restrictionName = dynamic_cast<MetadataIdentifier const*>(currentSection->GetItem("Restriction"))->GetValue();
					if (restrictionName == "Unrestricted")
					{
						restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED;
					}
					else if (restrictionName == "Integers")
					{
						restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_INTEGERS;
					}
					else if (restrictionName == "PowersOfTwo")
					{
						restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_2;
					}
					else if (restrictionName == "PowersOfTen")
					{
						restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_10;
					}
					else
					{
						throw compiler::IncorrectParameterDefinitionException(location, name);
					}
				}

				parameter = new NumberScriptParameter(name, label, description, defaultValue, min, max, restriction);
				break;
			}
		case SCRIPT_PARAMETER_TYPE_ENUM: 
		default:
			throw InternalErrorException("Unknown parameter type.");
		}

		table.AddItem(name, parameter);
	}
}

ScriptParameters* CompiledScript::CreateScriptParameters() const
{
	auto_ptr<ScriptParameters> table(new ScriptParameters());

	this->CreateScriptParameters(*table.get());

	return table.release();
}