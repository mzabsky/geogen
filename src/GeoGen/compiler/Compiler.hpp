#pragma once

#include <string>
#include <vector>

#include "CompiledScript.hpp"
#include "../runtime/VariableDefinition.hpp"
#include "../runtime/FunctionDefinition.hpp"
#include "../runtime/TypeDefinition.hpp"
#include "../runtime/SymbolDefinitionTable.hpp"

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
				CompiledScript const* CompileScript(std::string const& code) const;
		};
	}
}