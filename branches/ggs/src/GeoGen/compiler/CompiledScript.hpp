#pragma once

#include <vector>
#include <string>

#include "../runtime/VariableDefinition.hpp"
#include "../runtime/FunctionDefinition.hpp"
#include "../runtime/TypeDefinition.hpp"
#include "../runtime/SymbolDefinitionTable.hpp"
#include "../runtime/SymbolNameTable.hpp"
#include "../runtime/CodeBlock.hpp"
#include "MetadataValue.hpp"
#include "MetadataKeyValueCollection.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class CompiledScript
		{
			private:	
				runtime::SymbolNameTable symbolNameTable;

				runtime::SymbolDefinitionTable<runtime::VariableDefinition> globalVariableDefinitions;
				runtime::SymbolDefinitionTable<runtime::FunctionDefinition> globalFunctionDefinitions;
				runtime::SymbolDefinitionTable<runtime::TypeDefinition> typeDefinitions;

				runtime::CodeBlock rootCodeBlock;

				MetadataKeyValueCollection* metadata;
			public:
				CompiledScript();
				~CompiledScript();

				inline runtime::SymbolNameTable const& GetSymbolNameTable() const { return this->symbolNameTable; }
				inline runtime::SymbolNameTable& GetSymbolNameTable() { return this->symbolNameTable; }

				inline MetadataKeyValueCollection const* GetMetadata() const { return this->metadata; }
				inline MetadataKeyValueCollection* GetMetadata() { return this->metadata; }

				inline void SetMetadata(MetadataKeyValueCollection* metadata) { this->metadata = metadata; }

				inline runtime::SymbolDefinitionTable<runtime::VariableDefinition> const& GetGlobalVariableDefinitions() const { return this->globalVariableDefinitions; }
				inline runtime::SymbolDefinitionTable<runtime::VariableDefinition>& GetGlobalVariableDefinitions() { return this->globalVariableDefinitions; }

				inline runtime::SymbolDefinitionTable<runtime::FunctionDefinition> const& GetGlobalFunctionDefinitions() const { return this->globalFunctionDefinitions; }
				inline runtime::SymbolDefinitionTable<runtime::FunctionDefinition>& GetGlobalFunctionDefinitions() { return this->globalFunctionDefinitions; }

				inline runtime::SymbolDefinitionTable<runtime::TypeDefinition> const& GetTypeDefinitions() const { return this->typeDefinitions; }
				inline runtime::SymbolDefinitionTable<runtime::TypeDefinition>& GetTypeDefinitions() { return this->typeDefinitions; }

				inline runtime::CodeBlock& GetRootCodeBlock() { return this->rootCodeBlock; }
				inline runtime::CodeBlock const& GetRootCodeBlock() const { return this->rootCodeBlock; }
		};
	}
}