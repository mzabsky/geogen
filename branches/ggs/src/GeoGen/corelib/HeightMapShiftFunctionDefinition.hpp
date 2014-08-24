#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.Shift. </summary>
		class HeightMapShiftFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapShiftFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Shift"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}