#pragma once

#include <string>
#include <vector>

#include "../runtime/NativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class AssignmentOperatorFunctionDefinition : public runtime::NativeFunctionDefinition
		{
		private:
		public:
			AssignmentOperatorFunctionDefinition() : NativeFunctionDefinition("=") {};

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}