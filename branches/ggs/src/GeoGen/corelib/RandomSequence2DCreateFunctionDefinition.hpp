#pragma once

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for RandomSequence2D.Create.
		class RandomSequence2DCreateFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			RandomSequence2DCreateFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Create"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* type, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; };
		};
	}
}