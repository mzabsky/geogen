#pragma once

#include <vector>

#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class IncrementDecrementOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			enum Operator
			{
				PLUS_PLUS_PRE,
				PLUS_PLUS_POST,
				MINUS_MINUS_PRE,
				MINUS_MINUS_POST
			};
		private:
			Operator op;

			IncrementDecrementOperatorFunctionDefinition(String const& name, Operator op) : GlobalNativeFunctionDefinition(name), op(op)
			{
			};
		public:
			static IncrementDecrementOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}