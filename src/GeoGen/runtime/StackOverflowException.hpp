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