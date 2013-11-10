#include <string>

#include "..\Grammar\output\GeoGenScriptLexer.hpp"

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
				CompiledScript CompileScript(std::string& code);
		};
	}
}