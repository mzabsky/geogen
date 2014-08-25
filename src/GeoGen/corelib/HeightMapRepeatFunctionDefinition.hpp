#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.Repeat. </summary>
		class HeightMapRepeatFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapRepeatFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Repeat"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; }
		};
	}
}