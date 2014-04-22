#pragma once

#include <string>
#include <vector>

#include "../runtime/CompiledScript.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class TypeDefinition;

		class Compiler
		{
			private:
				Compiler(const Compiler&); // Not copyable
			public:
				Compiler(); 
				runtime::CompiledScript* CompileScript(std::string const& code) const;
		};
	}
}