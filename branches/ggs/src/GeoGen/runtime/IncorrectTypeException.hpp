#pragma once

#include <string>
#include <stdexcept>

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class IncorrectTypeException : public RuntimeException
		{
		private:
			std::string expectedTypeName;
			std::string actualTypeName;
		public:
			explicit IncorrectTypeException(ErrorCode code, CodeLocation location, std::string const& expectedTypeName, std::string const& actualTypeName) :
				RuntimeException(code, location), expectedTypeName(expectedTypeName), actualTypeName(actualTypeName) {};

			inline std::string GetExpectedTypeName() const { return this->expectedTypeName; }
			inline std::string GetActualTypeName() const { return this->actualTypeName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Object of type \"" << this->GetExpectedTypeName() + "\" expected, got \"" << this->GetActualTypeName() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}