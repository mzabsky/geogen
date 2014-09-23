#pragma once

#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace testlib
	{
		/// Definition of AssertEquals.
		class AssertEqualsFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		private:
		public:
			AssertEqualsFunctionDefinition() : GlobalNativeFunctionDefinition(GG_STR("AssertEquals")) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}