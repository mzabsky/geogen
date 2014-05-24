#include <iostream>

#include "CompiledScript.hpp"
#include "Library.hpp"
#include "FunctionDefinition.hpp"
#include "VariableDefinition.hpp"
#include "TypeDefinition.hpp"
#include "..\compiler\IncorrectParameterDefinitionException.hpp"
#include "NumberScriptParameter.hpp"
#include "MetadataString.hpp"
#include "MetadataIdentifier.hpp"
#include "..\InternalErrorException.hpp"
#include "MetadataBoolean.hpp"
#include "BooleanScriptParameter.hpp"
#include "MetadataNumber.hpp"
#include "..\compiler\ScriptParameterTypeNotSpecifiedException.hpp"
#include "..\compiler\IncorrectScriptParameterTypeException.hpp"
#include "..\compiler\IncorrectScriptParameterAttributeTypeException.hpp"
#include "..\compiler\IncorrectMetadataValueTypeException.hpp"
#include "..\compiler\IncorrectScriptParameterValueRestrictionException.hpp"
#include "..\compiler\UndefinedMetadataIdentifierException.hpp"
#include "..\compiler\IncorrectMapSizeNumericValueException.hpp"
#include "..\compiler\MinGreaterThanMaxSizeException.hpp"

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

ScriptParameters CompiledScript::CreateScriptParameters() const
{
	unsigned defaultMapWidth = MAP_SIZE_INFINITE;
	unsigned minMapWidth = MAP_SIZE_MIN;
	unsigned maxMapWidth = MAP_SIZE_INFINITE;	
	unsigned defaultMapHeight = MAP_SIZE_INFINITE;
	unsigned minMapHeight = MAP_SIZE_MIN;
	unsigned maxMapHeight = MAP_SIZE_INFINITE;
	
	if (this->GetMetadata().ContainsItem("Width"))
	{	
		if (this->GetMetadata().GetItem("Width")->GetType() == METADATA_TYPE_IDENTIFIER)
		{
			string identifierName = dynamic_cast<MetadataIdentifier const*>(this->GetMetadata().GetItem("Width"))->GetValue();
			if (identifierName != "Infinite")
			{
				throw compiler::UndefinedMetadataIdentifierException(this->GetMetadata().GetItem("Width")->GetLocation(), identifierName);
			}

			defaultMapWidth = MAP_SIZE_INFINITE;
			minMapWidth = MAP_SIZE_MIN;
			maxMapWidth = MAP_SIZE_INFINITE;
		}		
		else if (this->GetMetadata().GetItem("Width")->GetType() == METADATA_TYPE_KEYVALUE_COLLECTION)
		{
			MetadataKeyValueCollection const* widthSection = dynamic_cast<MetadataKeyValueCollection const*>(this->GetMetadata().GetItem("Width"));

			if (widthSection->ContainsItem("Default"))
			{
				if (widthSection->GetItem("Default")->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(widthSection->GetItem("Default"))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(widthSection->GetItem("Default")->GetLocation(), numericValue);
					}

					defaultMapWidth = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(widthSection->GetItem("Default")->GetLocation(), "Width", "Default", MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(widthSection->GetItem("Default")->GetType()));
				}
			}

			if (widthSection->ContainsItem("Min"))
			{
				if (widthSection->GetItem("Min")->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(widthSection->GetItem("Min"))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(widthSection->GetItem("Min")->GetLocation(), numericValue);
					}

					minMapWidth = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(widthSection->GetItem("Min")->GetLocation(), "Width", "Min", MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(widthSection->GetItem("Min")->GetType()));
				}
			}

			if (widthSection->ContainsItem("Max"))
			{
				if (widthSection->GetItem("Max")->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(widthSection->GetItem("Max"))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(widthSection->GetItem("Max")->GetLocation(), numericValue);
					}

					maxMapWidth = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(widthSection->GetItem("Max")->GetLocation(), "Width", "Max", MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(widthSection->GetItem("Max")->GetType()));
				}
			}

			if (minMapWidth > maxMapWidth)
			{
				throw compiler::MinGreaterThanMaxSizeException(widthSection->GetItem("Min")->GetLocation(), DIMENSION_WIDTH);
			}
		}
		else
		{
			throw compiler::IncorrectMetadataValueTypeException(GGE1408_WidthNotKeyValueCollection, this->GetMetadata().GetItem("Width")->GetLocation(), "Width", "KeyValueCollection", MetadataTypeToString(this->GetMetadata().GetItem("Width")->GetType()));
		}
	}

	if (this->GetMetadata().ContainsItem("Height"))
	{
		if (this->GetMetadata().GetItem("Height")->GetType() == METADATA_TYPE_IDENTIFIER)
		{
			string identifierName = dynamic_cast<MetadataIdentifier const*>(this->GetMetadata().GetItem("Height"))->GetValue();
			if (identifierName != "Infinite")
			{
				throw compiler::UndefinedMetadataIdentifierException(this->GetMetadata().GetItem("Height")->GetLocation(), identifierName);
			}

			defaultMapHeight = MAP_SIZE_INFINITE;
			minMapHeight = MAP_SIZE_MIN;
			maxMapHeight = MAP_SIZE_INFINITE;
		}
		else if (this->GetMetadata().GetItem("Height")->GetType() == METADATA_TYPE_KEYVALUE_COLLECTION)
		{
			MetadataKeyValueCollection const* heightSection = dynamic_cast<MetadataKeyValueCollection const*>(this->GetMetadata().GetItem("Height"));

			if (heightSection->ContainsItem("Default"))
			{
				if (heightSection->GetItem("Default")->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(heightSection->GetItem("Default"))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(heightSection->GetItem("Default")->GetLocation(), numericValue);
					}

					defaultMapHeight = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(heightSection->GetItem("Default")->GetLocation(), "Height", "Default", MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(heightSection->GetItem("Default")->GetType()));
				}
			}

			if (heightSection->ContainsItem("Min"))
			{
				if (heightSection->GetItem("Min")->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(heightSection->GetItem("Min"))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(heightSection->GetItem("Min")->GetLocation(), numericValue);
					}

					minMapHeight = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(heightSection->GetItem("Min")->GetLocation(), "Height", "Min", MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(heightSection->GetItem("Min")->GetType()));
				}
			}

			if (heightSection->ContainsItem("Max"))
			{
				if (heightSection->GetItem("Max")->GetType() == METADATA_TYPE_NUMBER)
				{
					Number numericValue = dynamic_cast<MetadataNumber const*>(heightSection->GetItem("Max"))->GetValue();

					if (numericValue < MAP_SIZE_MIN || numericValue > MAP_SIZE_MAX || !IsNumberInt(numericValue))
					{
						throw compiler::IncorrectMapSizeNumericValueException(heightSection->GetItem("Max")->GetLocation(), numericValue);
					}

					maxMapHeight = NumberToInt(numericValue);
				}
				else
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(heightSection->GetItem("Max")->GetLocation(), "Height", "Max", MetadataTypeToString(METADATA_TYPE_STRING), MetadataTypeToString(heightSection->GetItem("Max")->GetType()));
				}
			}

			if (minMapHeight > maxMapHeight)
			{
				throw compiler::MinGreaterThanMaxSizeException(heightSection->GetItem("Min")->GetLocation(), DIMENSION_HEIGHT);
			}
		}
		else
		{
			throw compiler::IncorrectMetadataValueTypeException(GGE1409_HeightNotKeyValueCollection, this->GetMetadata().GetItem("Height")->GetLocation(), "Height", "KeyValueCollection", MetadataTypeToString(this->GetMetadata().GetItem("Height")->GetType()));
		}
	}

	ScriptParameters table(defaultMapWidth, defaultMapHeight, minMapWidth, minMapHeight, maxMapWidth, maxMapHeight);

	// This code is first invoked by the compiler, so it throws compiler exceptions.
	if (this->GetMetadata().ContainsItem("Parameters"))
	{	
		if (this->GetMetadata().GetItem("Parameters")->GetType() != METADATA_TYPE_KEYVALUE_COLLECTION)
		{
			throw compiler::IncorrectMetadataValueTypeException(GGE1402_ScriptParametersNotKeyValueCollection, this->GetMetadata().GetItem("Parameters")->GetLocation(), "Parameters", MetadataTypeToString(METADATA_TYPE_KEYVALUE_COLLECTION), MetadataTypeToString(this->GetMetadata().GetItem("Parameters")->GetType()));
		}

		MetadataKeyValueCollection const* parametersSection = dynamic_cast<MetadataKeyValueCollection const*>(this->GetMetadata().GetItem("Parameters"));

	
		for (MetadataKeyValueCollection::const_iterator it = parametersSection->Begin(); it != parametersSection->End(); it++)
		{		
			string name = it->first;
			CodeLocation location = it->second->GetLocation();
		
			if (it->second->GetType() != METADATA_TYPE_KEYVALUE_COLLECTION)
			{
				throw compiler::IncorrectMetadataValueTypeException(GGE1403_ScriptParameterNotKeyValueCollection, location, name, "KeyValueCollection", MetadataTypeToString(this->GetMetadata().GetItem("Parameters")->GetType()));
			}

			MetadataKeyValueCollection const* currentSection = dynamic_cast<MetadataKeyValueCollection const*>(it->second);

			string label;
			if (currentSection->ContainsItem("Label"))
			{
				if (currentSection->GetItem("Label")->GetType() != METADATA_TYPE_STRING)
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem("Label")->GetLocation(), name, "Label", MetadataTypeToString(currentSection->GetItem("Label")->GetType()), MetadataTypeToString(METADATA_TYPE_STRING));
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
					throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem("Description")->GetLocation(), name, "Description", MetadataTypeToString(currentSection->GetItem("Description")->GetType()), MetadataTypeToString(METADATA_TYPE_STRING));
				}

				description = dynamic_cast<MetadataString const*>(currentSection->GetItem("Description"))->GetValue();
			}

			ScriptParameterType parameterType = SCRIPT_PARAMETER_TYPE_NUMBER;
			if (currentSection->ContainsItem("Type"))
			{
				if (currentSection->GetItem("Type")->GetType() != METADATA_TYPE_IDENTIFIER)
				{
					throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem("Type")->GetLocation(), name, "Type", MetadataTypeToString(currentSection->GetItem("Type")->GetType()), MetadataTypeToString(METADATA_TYPE_IDENTIFIER));
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
					throw compiler::IncorrectScriptParameterTypeException(location, name, parameterTypeName);
				}
			}
			else
			{
				throw compiler::ScriptParameterTypeNotSpecifiedException(location, name);
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
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem("Default")->GetLocation(), name, "Default", MetadataTypeToString(currentSection->GetItem("Default")->GetType()), MetadataTypeToString(METADATA_TYPE_BOOLEAN));
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
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem("Default")->GetLocation(), name, "Default", MetadataTypeToString(currentSection->GetItem("Default")->GetType()), MetadataTypeToString(METADATA_TYPE_NUMBER));
						}

						defaultValue = dynamic_cast<MetadataNumber const*>(currentSection->GetItem("Default"))->GetValue();
					}

					Number min = 0;
					if (currentSection->ContainsItem("Min"))
					{
						if (currentSection->GetItem("Min")->GetType() != METADATA_TYPE_NUMBER)
						{
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem("Min")->GetLocation(), name, "Min", MetadataTypeToString(currentSection->GetItem("Min")->GetType()), MetadataTypeToString(METADATA_TYPE_NUMBER));
						}

						min = dynamic_cast<MetadataNumber const*>(currentSection->GetItem("Min"))->GetValue();
					}

					Number max = 100;
					if (currentSection->ContainsItem("Max"))
					{
						if (currentSection->GetItem("Max")->GetType() != METADATA_TYPE_NUMBER)
						{
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem("Max")->GetLocation(), name, "Max", MetadataTypeToString(currentSection->GetItem("Max")->GetType()), MetadataTypeToString(METADATA_TYPE_NUMBER));
						}

						max = dynamic_cast<MetadataNumber const*>(currentSection->GetItem("Max"))->GetValue();
					}

					ScriptParameterValueRestriction restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED;
					if (currentSection->ContainsItem("Restriction"))
					{
						if (currentSection->GetItem("Restriction")->GetType() != METADATA_TYPE_IDENTIFIER)
						{
							throw compiler::IncorrectScriptParameterAttributeTypeException(currentSection->GetItem("Restriction")->GetLocation(), name, "Restriction", MetadataTypeToString(currentSection->GetItem("Restriction")->GetType()), MetadataTypeToString(METADATA_TYPE_IDENTIFIER));
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
						else if (restrictionName == "PowersOf2")
						{
							restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_2;
						}
						else if (restrictionName == "PowersOf10")
						{
							restriction = SCRIPT_PARAMETER_VALUE_RESTRICTION_POWERS_OF_10;
						}
						else
						{
							throw compiler::IncorrectScriptParameterValueRestrictionException(location, name, restrictionName);
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
	return table;
}