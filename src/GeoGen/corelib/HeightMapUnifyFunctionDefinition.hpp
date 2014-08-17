#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.Unify. </summary>
		class HeightMapUnifyFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapUnifyFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Unify"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}