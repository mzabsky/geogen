#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightProfile.Pattern. </summary>
		class HeightProfilePatternFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightProfilePatternFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Pattern"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; }
		};
	}
}