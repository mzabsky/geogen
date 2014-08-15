#pragma once

#include <vector>

#include "FunctionDefinition.hpp"
#include "../CodeLocation.hpp"

using namespace std;
using namespace geogen::runtime;

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class ManagedObject;
		class TypeDefinition;

		enum MethodType
		{
			METHOD_TYPE_STATIC,
			METHOD_TYPE_INSTANCE
		};

		class MemberNativeFunctionDefinition : public FunctionDefinition
		{
		private:
			TypeDefinition const* owningTypeDefinition;
		public:
			MemberNativeFunctionDefinition(String const& name, TypeDefinition const* owningTypeDefinition) : FunctionDefinition(name), owningTypeDefinition(owningTypeDefinition){};

			virtual ManagedObject* CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, std::vector<ManagedObject*> arguments) const = 0;

			virtual void Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const;

			virtual MethodType GetMethodType() const = 0;

			inline TypeDefinition const* GetOwningTypeDefinition() const { return this->owningTypeDefinition; }

			virtual void Serialize(IOStream& stream) const;
		};
	}
}
