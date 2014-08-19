#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// <summary> Function definition for RandomSequence2D.Advance. </summary>
		class RandomSequence2DAdvanceFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			RandomSequence2DAdvanceFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Advance"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}