#pragma once

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for Coordinate.FromNumber.
		class CoordinateFromNumberFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			CoordinateFromNumberFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("FromNumber"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* type, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; };
		};
	}
}