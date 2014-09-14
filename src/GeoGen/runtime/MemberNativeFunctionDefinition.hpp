#pragma once

#include <vector>

#include "FunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class ManagedObject;
		class TypeDefinition;

		/// Type of a member function - static vs. instance.
		enum MethodType
		{
			METHOD_TYPE_STATIC,
			METHOD_TYPE_INSTANCE
		};

		/// A member native function definition.
		class MemberNativeFunctionDefinition : public FunctionDefinition
		{
		private:
			TypeDefinition const* owningTypeDefinition;
		public:
			MemberNativeFunctionDefinition(String const& name, runtime::TypeDefinition const* owningTypeDefinition) : FunctionDefinition(name), owningTypeDefinition(owningTypeDefinition){};

			/// Method invoked when the function is called from a script. It is a higher level replacement for FunctionDefinition::Call.
			/// @param location The code location.
			/// @param vm The virtual machine.
			/// @param instance Instance on which the function is being called, if it is being called as a member method.
			/// @param arguments The arguments.
			/// @return The return value.
			virtual ManagedObject* CallNative(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, std::vector<ManagedObject*> arguments) const = 0;

			virtual void Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const;

			/// Gets method type.
			/// @return The method type.
			virtual runtime::MethodType GetMethodType() const = 0;

			/// Gets type definition of the type owning this method.
			/// @return The owning type definition.
			inline TypeDefinition const* GetOwningTypeDefinition() const { return this->owningTypeDefinition; }

			virtual void Serialize(IOStream& stream) const;
		};
	}
}
