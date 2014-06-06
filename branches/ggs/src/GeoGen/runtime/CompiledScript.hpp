#pragma once

#include <vector>

#include "../String.hpp"
#include "CodeBlock.hpp"
#include "MetadataValue.hpp"
#include "MetadataKeyValueCollection.hpp"
#include "../corelib/CoreLibrary.hpp"
#include "ScriptParameters.hpp"

namespace geogen 
{
	namespace runtime
	{
		class TypeDefinition;
		class VariableDefinition;
		class FunctionDefinition;
		class Library;

		class CompiledScript
		{
			private:	
				//SymbolNameTable symbolNameTable;

				SymbolDefinitionTable<VariableDefinition> globalVariableDefinitions;
				SymbolDefinitionTable<FunctionDefinition> globalFunctionDefinitions;
				SymbolDefinitionTable<TypeDefinition> typeDefinitions;

				std::vector<FunctionDefinition*> ownedFunctionDefinitions;
				std::vector<TypeDefinition*> ownedTypeDefinitions;

				CodeBlock rootCodeBlock;
				//CodeBlock initializationCodeBlock;

				MetadataKeyValueCollection metadata;

				corelib::CoreLibrary coreLibrary;
			public:
				static const String MAIN_FUNCTION_NAME;

				CompiledScript();
				~CompiledScript();

				//inline SymbolNameTable const& GetSymbolNameTable() const { return this->symbolNameTable; }
				//inline SymbolNameTable& GetSymbolNameTable() { return this->symbolNameTable; }				

				ScriptParameters CreateScriptParameters() const;

				inline MetadataKeyValueCollection const& GetMetadata() const { return this->metadata; }
				inline MetadataKeyValueCollection& GetMetadata() { return this->metadata; }

				//inline void SetMetadata(MetadataKeyValueCollection* metadata) { this->metadata = metadata; }

				inline SymbolDefinitionTable<VariableDefinition> const& GetGlobalVariableDefinitions() const { return this->globalVariableDefinitions; }
				//inline SymbolDefinitionTable<VariableDefinition>& GetGlobalVariableDefinitions() { return this->globalVariableDefinitions; }

				inline SymbolDefinitionTable<FunctionDefinition> const& GetGlobalFunctionDefinitions() const { return this->globalFunctionDefinitions; }
				//inline SymbolDefinitionTable<FunctionDefinition>& GetGlobalFunctionDefinitions() { return this->globalFunctionDefinitions; }

				inline SymbolDefinitionTable<TypeDefinition> const& GetTypeDefinitions() const { return this->typeDefinitions; }
				//inline SymbolDefinitionTable<TypeDefinition>& GetTypeDefinitions() { return this->typeDefinitions; }

				/*inline CodeBlock& GetRootCodeBlock() { return this->rootCodeBlock; }
				inline CodeBlock const& GetRootCodeBlock() const { return this->rootCodeBlock; }*/

				/*inline CodeBlock& GetInitializationCodeBlock() { return this->initializationCodeBlock; }
				inline CodeBlock const& GetInitializationCodeBlock() const { return this->initializationCodeBlock; }*/

				bool AddGlobalFunctionDefinition(FunctionDefinition* functionDefintion);
				bool AddTypeDefinition(TypeDefinition* typeDefinition);

				void AddLibrary(Library const* library);
		};
	}
}