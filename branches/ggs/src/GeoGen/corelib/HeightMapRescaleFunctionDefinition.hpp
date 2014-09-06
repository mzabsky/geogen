#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightMap.Rescale. </summary>
		class HeightMapRescaleFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapRescaleFunctionDefinition(runtime::TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Rescale"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual runtime::MethodType GetMethodType() const { return runtime::METHOD_TYPE_INSTANCE; }
		};
	}
}