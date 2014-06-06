#pragma once

#include "../CodeLocation.hpp"
#include "../String.hpp"

#include <vector>

namespace geogen 
{
	enum FunctionType
	{
		FUNCTION_TYPE_FUNCTION,
        FUNCTION_TYPE_OPERATOR
	};

	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;
		class TypeDefinition;

		class FunctionDefinition
		{
		private:
			String name;
		public:
			FunctionDefinition(String const& name) { this->name = name; }
			virtual ~FunctionDefinition() {};

			inline String const& GetName() const { return this->name; };

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_FUNCTION; }

			virtual void Call(CodeLocation location, VirtualMachine* vm, ManagedObject* instance, unsigned numberOfArguments) const = 0;

			void CheckArguments(CodeLocation location, std::vector<TypeDefinition const*> expectedTypes, std::vector<ManagedObject*> actualArguments) const;
			void CheckArguments(CodeLocation location, unsigned expectedArgumentCount, std::vector<ManagedObject*> actualArguments) const;
		};
	}
}