#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class IncorrectMetadataValueTypeException : public CompilerException
		{
		private:
			String valueName;
			String expectedType;
			String actualType;
		public:
			IncorrectMetadataValueTypeException(ErrorCode errorCode, CodeLocation location, String const& valueName, String const& expectedType, String const& actualType) :
				CompilerException(errorCode, location), valueName(valueName), expectedType(expectedType), actualType(actualType) {};

			inline String GetValueName() const { return this->valueName; }

			inline String GetExpectedType() const { return this->expectedType; }

			inline String GetActualType() const { return this->actualType; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Metadata value \"" << valueName << "\" expected to be of type \"" << expectedType << "\", had \"" << actualType << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}