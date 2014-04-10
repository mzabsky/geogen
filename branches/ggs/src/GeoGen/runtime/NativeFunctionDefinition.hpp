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
		class DynamicObject;
		class TypeDefinition;

		class NativeFunctionDefinition : public FunctionDefinition
		{
		public:
			NativeFunctionDefinition(std::string const& name) : FunctionDefinition(name) {};

			virtual void Call(CodeLocation location, VirtualMachine* vm, unsigned numberOfArguments) const;

			virtual DynamicObject* CallNative(CodeLocation location, VirtualMachine* vm, std::vector<DynamicObject*> arguments) const = 0;

			void CheckArguments(CodeLocation location, std::vector<TypeDefinition const*> expectedTypes, std::vector<DynamicObject*> actualArguments) const;
			void CheckArguments(CodeLocation location, unsigned expectedArgumentCount, std::vector<DynamicObject*> actualArguments) const;
		};
	}
}