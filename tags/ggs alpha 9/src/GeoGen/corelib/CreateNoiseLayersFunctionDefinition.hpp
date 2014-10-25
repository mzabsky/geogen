#pragma once

#include <vector>

#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class CreateNoiseLayersFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			CreateNoiseLayersFunctionDefinition() : GlobalNativeFunctionDefinition(GG_STR("CreateNoiseLayers")) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}