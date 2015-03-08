#pragma once

#include <vector>

#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class RelativeCoordinateOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		private:
		public:
			RelativeCoordinateOperatorFunctionDefinition() : GlobalNativeFunctionDefinition(GG_STR("@")) {};

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}