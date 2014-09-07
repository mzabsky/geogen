#pragma once

#include <vector>

#include "../String.hpp"
#include "../corelib/NumberTypeDefinition.hpp"
#include "../runtime/GlobalNativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		/// Definition for operators *, /, %, +, -, << and >>.
		class ArithmeticAssignmentOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			enum Operator
			{
				MULTIPLICATION,
				DIVISION,
				MODULO,
				ADDITION,
				SUBTRACTION,
				BIT_SHIFT_LEFT,
				BIT_SHIFT_RIGHT
			};
		private:
			Operator op;

			ArithmeticAssignmentOperatorFunctionDefinition(String const& name, Operator op) : GlobalNativeFunctionDefinition(name), op(op)
			{
			};
		public:
			static ArithmeticAssignmentOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}