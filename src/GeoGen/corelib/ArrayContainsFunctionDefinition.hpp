#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"
#include "ArrayTypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayContainsFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			enum Method
			{
				CONTAINS_KEY,
				CONTAINS_VALUE
			};
		private:
			Method method;

			ArrayContainsFunctionDefinition(String const& name, Method method, const TypeDefinition* owningType) : MemberNativeFunctionDefinition(name, owningType), method(method) {};
		public:
			static ArrayContainsFunctionDefinition* Create(Method method, TypeDefinition const* owningType);

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual MethodType GetMethodType() const { return METHOD_TYPE_INSTANCE; };
		};
	}
}