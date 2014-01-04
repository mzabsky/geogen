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
		class ScriptFunctionDefinition : public FunctionDefinition
		{
		private:
			int parameterCount;
			SymbolDefinitionTable<VariableDefinition> localVariableDefinitions;
			CodeBlock rootCodeBlock;
		public:
			ScriptFunctionDefinition(std::string const& name, int parameterCount) : FunctionDefinition(name) 
			{
				this->parameterCount = parameterCount;
			}

			inline int GetParameterCount() const { return this->parameterCount; }

			inline CodeBlock& GetRootCodeBlock() { return this->rootCodeBlock; }
			inline CodeBlock const& GetRootCodeBlock() const { return this->rootCodeBlock; }
		};
	}
}