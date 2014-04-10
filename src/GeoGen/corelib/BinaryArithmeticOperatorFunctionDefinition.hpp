#pragma once

#include <string>
#include <vector>

#include "../runtime/NumberTypeDefinition.hpp"
#include "../runtime/NativeFunctionDefinition.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace corelib
	{
		class BinaryArithmeticOperatorFunctionDefinition : public runtime::NativeFunctionDefinition
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
			typedef Number(*Function)(CodeLocation location, Number, Number);

			static Number CallOperatorMultiplication(CodeLocation location, Number, Number);
			static Number CallOperatorDivision(CodeLocation location, Number, Number);
			static Number CallOperatorModulo(CodeLocation location, Number, Number);
			static Number CallOperatorAddition(CodeLocation location, Number, Number);
			static Number CallOperatorSubtraction(CodeLocation location, Number, Number);
			static Number CallOperatorBitShiftLeft(CodeLocation location, Number, Number);
			static Number CallOperatorBitShiftRight(CodeLocation location, Number, Number);

			Function function;

			BinaryArithmeticOperatorFunctionDefinition(std::string const& name, Function function) : NativeFunctionDefinition(name), function(function)
			{
			};
		public:			
			static BinaryArithmeticOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::DynamicObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::DynamicObject*> arguments) const;


		};
	}
}