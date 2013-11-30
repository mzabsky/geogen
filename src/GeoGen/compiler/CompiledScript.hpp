#pragma once

#include <vector>
#include <string>

#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"
#include "TypeDefinition.hpp"
#include "SymbolDefinitionTable.hpp"
#include "SymbolNameTable.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class CompiledScript
		{
			private:	
				SymbolNameTable symbolNameTable;

				SymbolDefinitionTable<VariableDefinition> variableDefinitions;
				SymbolDefinitionTable<FunctionDefinition> functionDefinitions;
				SymbolDefinitionTable<TypeDefinition> typeDefinitions;

			public:
				inline SymbolNameTable const* GetSymbolNameTable() const { return &this->symbolNameTable; }
				inline SymbolNameTable* GetSymbolNameTable() { return &this->symbolNameTable; }

				inline SymbolDefinitionTable<VariableDefinition> const* GetVariableDefinitions() const { return &this->variableDefinitions; }
				inline SymbolDefinitionTable<VariableDefinition>* GetVariableDefinitions() { return &this->variableDefinitions; }

				inline SymbolDefinitionTable<FunctionDefinition> const* GetFunctionDefinitions() const { return &this->functionDefinitions; }
				inline SymbolDefinitionTable<FunctionDefinition>* GetFunctionDefinitions() { return &this->functionDefinitions; }

				inline SymbolDefinitionTable<TypeDefinition> const* GetTypeDefinitions() const { return &this->typeDefinitions; }
				inline SymbolDefinitionTable<TypeDefinition>* GetTypeDefinitions() { return &this->typeDefinitions; }
		};
	}
}