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