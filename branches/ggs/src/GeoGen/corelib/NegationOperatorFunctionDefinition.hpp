#pragma once

#include <string>
#include <vector>

#include "../runtime/NativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class NegationOperatorFunctionDefinition : public runtime::NativeFunctionDefinition
		{
		public:
			NegationOperatorFunctionDefinition() : NativeFunctionDefinition("!") {};

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::DynamicObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::DynamicObject*> arguments) const;
		};
	}
}