#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightProfile.Gradient. </summary>
		class HeightProfileGradientFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightProfileGradientFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Gradient"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; }
		};
	}
}