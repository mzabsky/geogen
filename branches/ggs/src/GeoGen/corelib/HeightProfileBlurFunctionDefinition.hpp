#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for HeightProfile.Blur.
		class HeightProfileBlurFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightProfileBlurFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Blur"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}