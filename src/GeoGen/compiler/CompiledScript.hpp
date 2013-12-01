#pragma once

#include <vector>
#include <string>

#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"
#include "TypeDefinition.hpp"
#include "SymbolDefinitionTable.hpp"
#include "SymbolNameTable.hpp"
#include "CodeBlock.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class CompiledScript
		{
			private:	
				SymbolNameTable symbolNameTable;

				SymbolDefinitionTable<VariableDefinition> globalVariableDefinitions;
				SymbolDefinitionTable<FunctionDefinition> globalFunctionDefinitions;
				SymbolDefinitionTable<TypeDefinition> typeDefinitions;

				CodeBlock rootCodeBlock;
			public:
				inline SymbolNameTable const* GetSymbolNameTable() const { return &this->symbolNameTable; }
				inline SymbolNameTable* GetSymbolNameTable() { return &this->symbolNameTable; }

				inline SymbolDefinitionTable<VariableDefinition> const* GetGlobalVariableDefinitions() const { return &this->globalVariableDefinitions; }
				inline SymbolDefinitionTable<VariableDefinition>* GetGlobalVariableDefinitions() { return &this->globalVariableDefinitions; }

				inline SymbolDefinitionTable<FunctionDefinition> const* GetGlobalFunctionDefinitions() const { return &this->globalFunctionDefinitions; }
				inline SymbolDefinitionTable<FunctionDefinition>* GetGlobalFunctionDefinitions() { return &this->globalFunctionDefinitions; }

				inline SymbolDefinitionTable<TypeDefinition> const* GetTypeDefinitions() const { return &this->typeDefinitions; }
				inline SymbolDefinitionTable<TypeDefinition>* GetTypeDefinitions() { return &this->typeDefinitions; }

				inline CodeBlock& GetRootCodeBlock() { return this->rootCodeBlock; }
				inline CodeBlock const& GetRootCodeBlock() const { return this->rootCodeBlock; }
		};
	}
}