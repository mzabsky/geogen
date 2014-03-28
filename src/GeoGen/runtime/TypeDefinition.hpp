#pragma once

#include <string>

#include "SymbolDefinitionTable.hpp"
#include "VirtualMachine.hpp"
#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"

namespace geogen 
{
	namespace runtime
	{
		class DynamicObject;
		class StaticObject;
		class VariableDefinition;
		class FunctionDefinition;

		class TypeDefinition
		{
		private:
			std::string name;

			SymbolDefinitionTable<VariableDefinition> variableDefinitions;
			SymbolDefinitionTable<FunctionDefinition> functionDefinitions;
			SymbolDefinitionTable<VariableDefinition> staticVariableDefinitions;
			SymbolDefinitionTable<FunctionDefinition> staticFunctionDefinitions;
		protected:
			TypeDefinition(std::string const& name) : name(name) {}
		public:			
			virtual ~TypeDefinition() {};
			
			inline std::string const& GetName() const { return this->name; };			

			inline SymbolDefinitionTable<VariableDefinition>& GetVariableDefinitions() { return this->variableDefinitions; };
			inline SymbolDefinitionTable<VariableDefinition> const& GetVariableDefinitions() const { return this->variableDefinitions; };

			inline SymbolDefinitionTable<FunctionDefinition>& GetFunctionDefinitions() { return this->functionDefinitions; };
			inline SymbolDefinitionTable<FunctionDefinition> const& GetFunctionDefinitions() const { return this->functionDefinitions; };

			inline SymbolDefinitionTable<VariableDefinition>& GetStaticVariableDefinitions() { return this->variableDefinitions; };
			inline SymbolDefinitionTable<VariableDefinition> const& GetStaticVariableDefinitions() const { return this->variableDefinitions; };

			inline SymbolDefinitionTable<FunctionDefinition>& GetStaticFunctionDefinitions() { return this->functionDefinitions; };
			inline SymbolDefinitionTable<FunctionDefinition> const& GetStaticFunctionDefinitions() const { return this->functionDefinitions; };

			StaticObject* CreateStaticObject() const;

			virtual void Initialize(VirtualMachine& vm);

			virtual bool InstanceLessThan(DynamicObject const* a, DynamicObject const* b) const = 0;
			virtual bool InstanceEqualsTo(DynamicObject const* a, DynamicObject const* b) const;

			virtual DynamicObject* Copy(DynamicObject* a) const = 0;
		};
	}
}