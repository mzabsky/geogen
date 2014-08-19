#pragma once

#include <vector>

#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"
#include "../random/RandomSequence.hpp"

namespace geogen
{
	namespace corelib
	{
		class RandomFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			RandomFunctionDefinition() : GlobalNativeFunctionDefinition(GG_STR("Random")) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}