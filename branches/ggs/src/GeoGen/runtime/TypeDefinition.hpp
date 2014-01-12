#pragma once

#include <string>

#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"
#include "SymbolDefinitionTable.hpp"

namespace geogen 
{
	namespace runtime
	{
		class DynamicObject;
		class VirtualMachine;

		class TypeDefinition
		{
		private:
			std::string name;

			SymbolDefinitionTable<VariableDefinition> variableDefinitions;
			SymbolDefinitionTable<FunctionDefinition> functionDefinitions;
		protected:
			TypeDefinition(std::string const& name) : name(name) {}
		public:			
			inline std::string const& GetName() const { return this->name; };
			
			inline SymbolDefinitionTable<VariableDefinition>& GetVariableDefinitions() { return this->variableDefinitions; };
			inline SymbolDefinitionTable<VariableDefinition> const& GetVariableDefinitions() const { return this->variableDefinitions; };

			inline SymbolDefinitionTable<FunctionDefinition>& GetFunctionDefinitions() { return this->functionDefinitions; };
			inline SymbolDefinitionTable<FunctionDefinition> const& GetFunctionDefinitions() const { return this->functionDefinitions; };

			virtual bool InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const = 0;
			virtual bool InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const;

			virtual bool Copy(VirtualMachine& vm, DynamicObject const* a) const = 0;
		};
	}
}