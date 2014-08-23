#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for HeightProfile.Move.
		class HeightProfileMoveFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightProfileMoveFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Move"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; }
		};
	}
}