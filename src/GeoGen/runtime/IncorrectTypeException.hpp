#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class IncorrectTypeException : public RuntimeException
		{
		private:
			String expectedTypeName;
			String actualTypeName;
		public:
			explicit IncorrectTypeException(ErrorCode code, CodeLocation location, String const& expectedTypeName, String const& actualTypeName) :
				RuntimeException(code, location), expectedTypeName(expectedTypeName), actualTypeName(actualTypeName) {};

			inline String GetExpectedTypeName() const { return this->expectedTypeName; }
			inline String GetActualTypeName() const { return this->actualTypeName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Object of type \"" << this->GetExpectedTypeName() << "\" expected, got \"" << this->GetActualTypeName() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}