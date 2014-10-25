#pragma once

#include <vector>

#include "../runtime/MemberNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"
#include "ArrayTypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		/// Definition for function Array.Create.
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

			ArrayContainsFunctionDefinition(String const& name, Method method, runtime::TypeDefinition const* owningType) : MemberNativeFunctionDefinition(name, owningType), method(method) {};
		public:
			static ArrayContainsFunctionDefinition* Create(Method method, runtime::TypeDefinition const* owningType);

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual runtime::MethodType GetMethodType() const { return runtime::METHOD_TYPE_INSTANCE; };
		};
	}
}