#pragma once

#include <string>
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

		class GlobalNativeFunctionDefinition : public FunctionDefinition
		{
		public:
			GlobalNativeFunctionDefinition(String const& name) : FunctionDefinition(name) {};

			virtual void Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const;

			virtual ManagedObject* CallNative(CodeLocation location, VirtualMachine* vm, std::vector<ManagedObject*> arguments) const = 0;
		};
	}
}