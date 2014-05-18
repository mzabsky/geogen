#pragma once

#include <string>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class IncorrectMetadataValueTypeException : public CompilerException
		{
		private:
			std::string valueName;
			std::string expectedType;
			std::string actualType;
		public:
			IncorrectMetadataValueTypeException(ErrorCode errorCode, CodeLocation location, std::string const& valueName, std::string const& expectedType, std::string const& actualType) :
				CompilerException(errorCode, location), valueName(valueName), expectedType(expectedType), actualType(actualType) {};

			inline std::string GetValueName() const { return this->valueName; }

			inline std::string GetExpectedType() const { return this->expectedType; }

			inline std::string GetActualType() const { return this->actualType; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Metadata value \"" << valueName << "\" expected to be of type \"" << expectedType << "\", had \"" << actualType << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}