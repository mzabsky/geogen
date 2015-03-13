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
		class LogicalOperatorFunctionDefinition : public runtime::GlobalNativeFunctionDefinition
		{
		public:
			enum Operator
			{
				LOGICAL_AND,
				LOGICAL_OR
			};
		private:
			typedef bool(*Function)(CodeLocation location, bool, bool);

			static bool CallOperatorLogicalAnd(CodeLocation location, bool, bool);
			static bool CallOperatorLogicalOr(CodeLocation location, bool, bool);

			Function function;

			LogicalOperatorFunctionDefinition(String const& name, Function function) : GlobalNativeFunctionDefinition(name), function(function)
			{
			};
		public:
			static LogicalOperatorFunctionDefinition* Create(Operator op);

			virtual FunctionType GetFunctionType() const { return FUNCTION_TYPE_OPERATOR; }

			virtual runtime::ManagedObject* CallNative(CodeLocation location, runtime::VirtualMachine* vm, std::vector<runtime::ManagedObject*> arguments) const;
		};
	}
}