#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.Pattern. </summary>
		class HeightMapPatternFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapPatternFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Pattern"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; }
		};
	}
}