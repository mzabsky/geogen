#pragma once

#include <string>
#include <vector>

#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class AssignmentOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		private:
		public:
			AssignmentOperatorFunctionDefinition() : GlobalNativeFunctionDefinition("=") {};

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}