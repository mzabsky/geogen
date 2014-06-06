#pragma once

#include "../String.hpp"

#include "SymbolDefinitionTable.hpp"

#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"

namespace geogen 
{
	namespace runtime
	{
		class ManagedObject;
		class StaticObject;
		class VariableDefinition;
		class FunctionDefinition;

		class TypeDefinition
		{
		private:
			String name;

			SymbolDefinitionTable<VariableDefinition> variableDefinitions;
			SymbolDefinitionTable<FunctionDefinition> functionDefinitions;
			SymbolDefinitionTable<VariableDefinition> staticVariableDefinitions;
			SymbolDefinitionTable<FunctionDefinition> staticFunctionDefinitions;
		protected:
			TypeDefinition(String const& name) : name(name) {}
		public:			
			virtual ~TypeDefinition() {};
			
			inline String const& GetName() const { return this->name; };			

			inline SymbolDefinitionTable<VariableDefinition>& GetVariableDefinitions() { return this->variableDefinitions; };
			inline SymbolDefinitionTable<VariableDefinition> const& GetVariableDefinitions() const { return this->variableDefinitions; };

			inline SymbolDefinitionTable<FunctionDefinition>& GetFunctionDefinitions() { return this->functionDefinitions; };
			inline SymbolDefinitionTable<FunctionDefinition> const& GetFunctionDefinitions() const { return this->functionDefinitions; };

			inline SymbolDefinitionTable<VariableDefinition>& GetStaticVariableDefinitions() { return this->variableDefinitions; };
			inline SymbolDefinitionTable<VariableDefinition> const& GetStaticVariableDefinitions() const { return this->variableDefinitions; };

			inline SymbolDefinitionTable<FunctionDefinition>& GetStaticFunctionDefinitions() { return this->functionDefinitions; };
			inline SymbolDefinitionTable<FunctionDefinition> const& GetStaticFunctionDefinitions() const { return this->functionDefinitions; };

			StaticObject* CreateStaticObject(VirtualMachine* vm) const;

			virtual void Initialize(VirtualMachine* vm) const;

			virtual bool IsEnumType() const { return false; };

			virtual bool InstanceLessThan(ManagedObject const* a, ManagedObject const* b) const;
			virtual bool InstanceEqualsTo(ManagedObject const* a, ManagedObject const* b) const;

			virtual ManagedObject* Copy(VirtualMachine* vm, ManagedObject* a) const = 0;
		};
	}
}