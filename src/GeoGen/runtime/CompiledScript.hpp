#pragma once

#include <vector>
#include <string>

#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"
#include "SymbolDefinitionTable.hpp"
#include "SymbolNameTable.hpp"
#include "CodeBlock.hpp"
#include "MetadataValue.hpp"
#include "MetadataKeyValueCollection.hpp"

namespace geogen 
{
	namespace runtime
	{
		class TypeDefinition;

		class CompiledScript
		{
			private:	
				SymbolNameTable symbolNameTable;

				SymbolDefinitionTable<VariableDefinition> globalVariableDefinitions;
				SymbolDefinitionTable<FunctionDefinition> globalFunctionDefinitions;
				SymbolDefinitionTable<TypeDefinition> typeDefinitions;

				CodeBlock rootCodeBlock;

				MetadataKeyValueCollection* metadata;
			public:
				CompiledScript();
				~CompiledScript();

				inline SymbolNameTable const& GetSymbolNameTable() const { return this->symbolNameTable; }
				inline SymbolNameTable& GetSymbolNameTable() { return this->symbolNameTable; }

				inline MetadataKeyValueCollection const* GetMetadata() const { return this->metadata; }
				inline MetadataKeyValueCollection* GetMetadata() { return this->metadata; }

				inline void SetMetadata(MetadataKeyValueCollection* metadata) { this->metadata = metadata; }

				inline SymbolDefinitionTable<VariableDefinition> const& GetGlobalVariableDefinitions() const { return this->globalVariableDefinitions; }
				inline SymbolDefinitionTable<VariableDefinition>& GetGlobalVariableDefinitions() { return this->globalVariableDefinitions; }

				inline SymbolDefinitionTable<FunctionDefinition> const& GetGlobalFunctionDefinitions() const { return this->globalFunctionDefinitions; }
				inline SymbolDefinitionTable<FunctionDefinition>& GetGlobalFunctionDefinitions() { return this->globalFunctionDefinitions; }

				inline SymbolDefinitionTable<TypeDefinition> const& GetTypeDefinitions() const { return this->typeDefinitions; }
				inline SymbolDefinitionTable<TypeDefinition>& GetTypeDefinitions() { return this->typeDefinitions; }

				inline CodeBlock& GetRootCodeBlock() { return this->rootCodeBlock; }
				inline CodeBlock const& GetRootCodeBlock() const { return this->rootCodeBlock; }
		};
	}
}