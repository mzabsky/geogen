/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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
		class BinaryArithmeticOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
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

			BinaryArithmeticOperatorFunctionDefinition(String const& name, Function function) : GlobalNativeFunctionDefinition(name), function(function)
			{
			};
		public:			
			static BinaryArithmeticOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}