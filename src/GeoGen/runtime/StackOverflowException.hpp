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

#include <stdexcept>

#include "RuntimeException.hpp"
#include "..\InternalErrorException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Stack types.
		enum StackType
		{
			/// ObjectStack. 
			STACK_TYPE_OBJECT,

			/// CallStack. 
			STACK_TYPE_CALL,

			/// CodeBlockStack.
			STACK_TYPE_CODE_BLOCK
		};

		/// Exception thrown when error geogen::GGE2501_CallStackOverflow, geogen::GGE2502_CodeBlockStackOverflow and geogen::GGE2503_ObjectStackOverflow occurs.
		class StackOverflowException : public RuntimeException
		{
		private:
			StackType stackType;
		public:

			/// Constructor.
			/// @param code The error code.
			/// @param location The code location.
			/// @param stackType Type of the stack.
			StackOverflowException(ErrorCode code, CodeLocation location, StackType stackType) :
				RuntimeException(code, location), stackType(stackType) {};

			virtual ~StackOverflowException() throw () {}

			/// Gets stack type.
			/// @return The stack type.
			inline StackType GetStackType() const { return this->stackType; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				switch (this->GetStackType())
				{
				case STACK_TYPE_CALL:
					ss << "Call";
				case STACK_TYPE_OBJECT:
					ss << "Object";
				case STACK_TYPE_CODE_BLOCK:
					ss << "Code block";
				default:
					throw InternalErrorException(GG_STR("Unknown stack type"));
					break;
				}

				ss << " stack overflow.";

				return ss.str();
			};
		};
	}
}