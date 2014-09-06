#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightProfile.Slice. </summary>
		class HeightProfileSliceFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightProfileSliceFunctionDefinition(runtime::TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Slice"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual runtime::MethodType GetMethodType() const { return runtime::METHOD_TYPE_STATIC; }
		};
	}
}