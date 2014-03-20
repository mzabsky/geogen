#pragma once

#include <vector>
#include <stack>
#include <map>

#include "TypeDefinition.hpp"
#include "FunctionDefinition.hpp" 
#include "VariableDefinition.hpp"

namespace geogen
{
	namespace runtime
	{
		class Library
		{
		private:
			SymbolDefinitionTable<VariableDefinition, true> globalVariableDefinitions;
			SymbolDefinitionTable<FunctionDefinition, true> globalFunctionDefinitions;
			SymbolDefinitionTable<TypeDefinition, true> typeDefinitions;

			Library(Library const& other) {};
			Library& operator=(Library const& other) {};
		public:			
			Library() {};
			virtual ~Library() {};

			inline SymbolDefinitionTable<VariableDefinition, true> const& GetGlobalVariableDefinitions() const { return this->globalVariableDefinitions; }
			//inline SymbolDefinitionTable<VariableDefinition>& GetGlobalVariableDefinitions() { return this->globalVariableDefinitions; }

			inline SymbolDefinitionTable<FunctionDefinition, true> const& GetGlobalFunctionDefinitions() const { return this->globalFunctionDefinitions; }
			//inline SymbolDefinitionTable<FunctionDefinition>& GetGlobalFunctionDefinitions() { return this->globalFunctionDefinitions; }

			inline SymbolDefinitionTable<TypeDefinition, true> const& GetTypeDefinitions() const { return this->typeDefinitions; }
			//inline SymbolDefinitionTable<TypeDefinition>& GetTypeDefinitions() { return this->typeDefinitions; }
		};
	}
}