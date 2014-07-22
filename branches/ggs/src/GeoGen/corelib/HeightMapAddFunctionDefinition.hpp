#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.Add. </summary>
		class HeightMapAddFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapAddFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Add"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}