#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
namespace geogen
{
	namespace corelib
	{
		/// Function definition for HeightMap.Noise. 
		class HeightMapNoiseFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			HeightMapNoiseFunctionDefinition(TypeDefinition const* type) : MemberNativeFunctionDefinition(GG_STR("Noise"), type) {};

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_STATIC; }
		};
	}
}