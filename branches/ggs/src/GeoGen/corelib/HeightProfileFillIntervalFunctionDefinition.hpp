#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for HeightProfile.FillInterval.
		class HeightProfileFillIntervalFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightProfileFillIntervalFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("FillInterval"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}