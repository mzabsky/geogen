#pragma once

#include <string>
#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"
#include "ArrayTypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayRemoveFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			enum Method
			{
				REMOVE_KEY,
				REMOVE_VALUE
			};
		private:
			Method method;

			ArrayRemoveFunctionDefinition(std::string const& name, Method method, const TypeDefinition* owningType) : MemberNativeFunctionDefinition(name, owningType), method(method) {};
		public:
			static ArrayRemoveFunctionDefinition* Create(Method method, TypeDefinition const* owningType);

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; };
		};
	}
}