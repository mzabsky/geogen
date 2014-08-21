#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.ProjectProfile. </summary>
		class HeightMapProjectProfileFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapProjectProfileFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("ProjectProfile"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; }
		};
	}
}