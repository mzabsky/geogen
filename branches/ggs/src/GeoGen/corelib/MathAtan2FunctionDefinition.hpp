#pragma once

#include <vector>

#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class MathAtan2FunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			MathAtan2FunctionDefinition() : GlobalNativeFunctionDefinition("Atan2") {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}