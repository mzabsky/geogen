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
		/// Definition for functions Array.Front and Array.Back.
		class ArrayFrontBackFunctionDefinition : public runtime::MemberNativeFunctionDefinition
		{
		public:
			enum Method
			{
				FRONT,
				BACK
			};
		private:
			Method method;

			ArrayFrontBackFunctionDefinition(String const& name, Method method, runtime::TypeDefinition const* owningType) : MemberNativeFunctionDefinition(name, owningType), method(method) {};
		public:
			static ArrayFrontBackFunctionDefinition* Create(Method method, runtime::TypeDefinition const* owningType);

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, runtime::ManagedObject* instance, std::vector<runtime::ManagedObject*> arguments) const;

			virtual runtime::MethodType GetMethodType() const { return runtime::METHOD_TYPE_INSTANCE; };
		};
	}
}