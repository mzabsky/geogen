#pragma once

#include <string>

#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"
#include "SymbolDefinitionTable.hpp"

namespace geogen 
{
	namespace runtime
	{
		class TypeDefinition
		{
		private:
			std::string name;

			SymbolDefinitionTable<VariableDefinition> variableDefinitions;
			SymbolDefinitionTable<FunctionDefinition> functionDefinitions;
		public:
			TypeDefinition(std::string const& name) : name(name) {}

			inline std::string const& GetName() const { return this->name; };
			
			inline SymbolDefinitionTable<VariableDefinition>& GetVariableDefinitions() { return this->variableDefinitions; };
			inline SymbolDefinitionTable<VariableDefinition> const& GetVariableDefinitions() const { return this->variableDefinitions; };

			inline SymbolDefinitionTable<FunctionDefinition>& GetFunctionDefinitions() { return this->functionDefinitions; };
			inline SymbolDefinitionTable<FunctionDefinition> const& GetFunctionDefinitions() const { return this->functionDefinitions; };
		};
	}
}