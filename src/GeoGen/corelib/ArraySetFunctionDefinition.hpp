#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"
#include "ArrayTypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArraySetFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		private:
		public:
			ArraySetFunctionDefinition(ArrayTypeDefinition const* owningType) : MemberNativeFunctionDefinition("[]=", owningType) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; };
		};
	}
}