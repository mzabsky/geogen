#pragma once

#include <vector>

#include "../compiler/CompilerConfiguration.hpp"
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
			const CompilerConfiguration configuration;
		private:
			Compiler(const Compiler&); // Not copyable
		public:

			/// Default constructor.
			/// @param configuration (Optional) Compiler configuration.
			Compiler(CompilerConfiguration configuration = CompilerConfiguration()); 

			/// Compiles script string into a runtime::CompiledString.
			/// @param code The code.
			/// @return A compiled script.
			runtime::CompiledScript* CompileScript(String const& code) const;

			/// Gets the configuration of the compiler.
			/// @return The configuration.
			inline CompilerConfiguration const& GetConfiguration() const { return this->configuration; }
		};
	}
}