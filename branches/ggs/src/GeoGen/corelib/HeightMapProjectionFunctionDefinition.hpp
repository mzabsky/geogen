#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.Projection. </summary>
		class HeightMapProjectionFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapProjectionFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Projection"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; }
		};
	}
}