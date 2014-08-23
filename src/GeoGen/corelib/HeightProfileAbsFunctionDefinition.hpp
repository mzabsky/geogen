#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for HeightProfile.Abs.
		class HeightProfileAbsFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightProfileAbsFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Abs"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}