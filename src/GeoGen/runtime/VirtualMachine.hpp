#pragma once

#include <vector>
#include <string>

#include "CompiledScript.hpp"

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;

		class VirtualMachine
		{
		private:
			CompiledScript const& compiledScript;
		public:
			VirtualMachine(CompiledScript const& compiledScript)
				: compiledScript(compiledScript) {};
			~VirtualMachine() {};

			inline CompiledScript const& GetCompiledScript() const { return this->compiledScript; };

			void RegisterObject(DynamicObject* object) { /* DUMMY */ };
			void UnregisterObject(DynamicObject* object) { /* DUMMY */ };
		};
	}
}