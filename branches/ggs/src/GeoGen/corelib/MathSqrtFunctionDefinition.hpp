#pragma once

#include <vector>

#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class MathSqrtFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			MathSqrtFunctionDefinition() : GlobalNativeFunctionDefinition(GG_STR("Sqrt")) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}