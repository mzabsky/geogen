#pragma once

#include <string>
#include <vector>

#include "FunctionDefinition.hpp"
#include "VariableDefinition.hpp"
#include "SymbolDefinitionTable.hpp"
#include "CodeBlock.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class ScriptFunctionDefinition : FunctionDefinition
		{
		private:
			int parameterCount;
			SymbolDefinitionTable<VariableDefinition> localVariableDefinitions;
			CodeBlock rootCodeBlock;
		public:
			ScriptFunctionDefinition(std::string name, int parameterCount) : FunctionDefinition(name) 
			{
				this->parameterCount = parameterCount;
			}

			inline int GetParameterCount() { return this->parameterCount; }

			inline SymbolDefinitionTable<VariableDefinition> const* GetLocalVariableDefinitions() const { return &this->localVariableDefinitions; }
			inline SymbolDefinitionTable<VariableDefinition>* GetLocalVariableDefinitions() { return &this->localVariableDefinitions; }			

			inline CodeBlock& GetRootCodeBlock() { return this->rootCodeBlock; }
			inline CodeBlock const& GetRootCodeBlock() const { return this->rootCodeBlock; }
		};
	}
}