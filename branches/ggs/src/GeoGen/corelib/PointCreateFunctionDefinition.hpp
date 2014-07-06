#pragma once

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for Point.Create.
		class PointCreateFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			PointCreateFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Create"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* type, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; };
		};
	}
}