#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for HeightProfile.Combine.
		class HeightProfileCombineFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightProfileCombineFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Combine"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}