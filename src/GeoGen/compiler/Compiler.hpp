#pragma once

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
		/// A script compiler based on ANTLR 3.
		class Compiler
		{
			private:
				Compiler(const Compiler&); // Not copyable
			public:
				/// Default constructor.
				Compiler(); 

				/// Compiles script string into a runtime::CompiledString.
				/// @param code The code.
				/// @return A compiled script.
				runtime::CompiledScript* CompileScript(String const& code) const;
		};
	}
}