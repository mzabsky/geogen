#pragma once

#include <string>

#include "CompiledScript.hpp"

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
				CompiledScript CompileScript(std::string& code);
		};
	}
}