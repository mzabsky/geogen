#pragma once

#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace testlib
	{
		class AssertEqualsFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		private:
		public:
			AssertEqualsFunctionDefinition() : GlobalNativeFunctionDefinition("AssertEquals") {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}