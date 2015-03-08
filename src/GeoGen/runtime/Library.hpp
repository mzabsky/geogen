#pragma once

#include <vector>
#include <stack>
#include <map>

#include "SymbolDefinitionTable.hpp"
#include "VariableDefinition.hpp"
#include "TypeDefinition.hpp"

namespace geogen
{
	namespace runtime
	{
		class TypeDefinition;
		class FunctionDefinition;
		class VariableDefinition;

		/// Collection of function, variable and type definitions.
		class Library
		{
		private:
			Library(Library const& other) {};
			Library& operator=(Library const& other) {};
		protected:
			SymbolDefinitionTable<VariableDefinition, true> globalVariableDefinitions;
			SymbolDefinitionTable<FunctionDefinition, true> globalFunctionDefinitions;
			SymbolDefinitionTable<TypeDefinition, true> typeDefinitions;
		public:			
			/// Default constructor.
			Library() {};

			/// Destructor.
			virtual ~Library() {};

			/// Gets global variable definitions.
			/// @return The global variable definitions.
			inline SymbolDefinitionTable<VariableDefinition, true> const& GetGlobalVariableDefinitions() const { return this->globalVariableDefinitions; }
			//inline SymbolDefinitionTable<VariableDefinition>& GetGlobalVariableDefinitions() { return this->globalVariableDefinitions; }

			/// Gets global function definitions.
			/// @return The global function definitions.
			inline SymbolDefinitionTable<FunctionDefinition, true> const& GetGlobalFunctionDefinitions() const { return this->globalFunctionDefinitions; }
			//inline SymbolDefinitionTable<FunctionDefinition>& GetGlobalFunctionDefinitions() { return this->globalFunctionDefinitions; }

			/// Gets type definitions.
			/// @return The type definitions.
			inline SymbolDefinitionTable<TypeDefinition, true> const& GetTypeDefinitions() const { return this->typeDefinitions; }
			//inline SymbolDefinitionTable<TypeDefinition>& GetTypeDefinitions() { return this->typeDefinitions; }
		};
	}
}