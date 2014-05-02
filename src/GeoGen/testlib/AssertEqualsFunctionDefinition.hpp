#pragma once

#include "../runtime/NativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace testlib
	{
		class AssertEqualsFunctionDefinition : public runtime::NativeFunctionDefinition
		{
		private:
		public:
			AssertEqualsFunctionDefinition() : NativeFunctionDefinition("AssertEquals") {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}