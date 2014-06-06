#pragma once

#include <string>
#include <vector>

#include "../runtime/CompiledScript.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class TypeDefinition;
	}

	namespace compiler 
	{		
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