#pragma once

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for Number.FromEnum.
		class NumberFromEnumFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			NumberFromEnumFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("FromEnum"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* type, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; };
		};
	}
}