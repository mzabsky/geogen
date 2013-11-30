#pragma once

#include <vector>
#include <string>

#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"
#include "TypeDefinition.hpp"
#include "SymbolDefinitionTable.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class CompiledScript
		{
			private:	
				std::vector<std::string> methodNameTable;

				SymbolDefinitionTable<VariableDefinition> variableDefinitions;
				SymbolDefinitionTable<FunctionDefinition> functionDefinitions;
				SymbolDefinitionTable<TypeDefinition> typeDefinitions;
		};
	}
}