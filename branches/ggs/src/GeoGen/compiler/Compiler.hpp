#pragma once

#include <string>
#include <vector>

#include "CompiledScript.hpp"
#include "VariableDefinition.hpp"
#include "FunctionDefinition.hpp"
#include "TypeDefinition.hpp"
#include "SymbolDefinitionTable.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class Compiler
		{
			private:
				Compiler(const Compiler&); // Not copyable
			public:
				Compiler(); 
				const CompiledScript CompileScript(std::string const& code) const;
		};
	}
}