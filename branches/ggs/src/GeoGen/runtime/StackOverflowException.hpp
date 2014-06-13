#pragma once

#include <stdexcept>

#include "RuntimeException.hpp"
#include "..\InternalErrorException.hpp"

namespace geogen
{
	namespace runtime
	{
		enum StackType
		{
			STACK_TYPE_OBJECT,
			STACK_TYPE_CALL,
			STACK_TYPE_CODE_BLOCK
		};

		class StackOverflowException : public RuntimeException
		{
		private:
			StackType stackType;
		public:
			StackOverflowException(ErrorCode code, CodeLocation location, StackType stackType) :
				RuntimeException(code, location), stackType(stackType) {};

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