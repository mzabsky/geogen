#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for HeightProfile.Flat. </summary>
		class HeightProfileFlatFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightProfileFlatFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Flat"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* type, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}