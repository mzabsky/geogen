#pragma once

#include <vector>

#include "../String.hpp"
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

			ArrayRemoveFunctionDefinition(String const& name, Method method, runtime::TypeDefinition const* owningType) : MemberNativeFunctionDefinition(name, owningType), method(method) {};
		public:
			static ArrayRemoveFunctionDefinition* Create(Method method, runtime::TypeDefinition const* owningType);

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual runtime::MethodType GetMethodType() const { return runtime::METHOD_TYPE_INSTANCE; };
		};
	}
}