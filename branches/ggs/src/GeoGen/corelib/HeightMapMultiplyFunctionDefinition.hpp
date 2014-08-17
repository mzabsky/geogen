#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.Multiply. </summary>
		class HeightMapMultiplyFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapMultiplyFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Multiply"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}