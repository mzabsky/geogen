#pragma once

#include <string>
#include <vector>

#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class NegationOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			NegationOperatorFunctionDefinition() : GlobalNativeFunctionDefinition("!") {};

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}