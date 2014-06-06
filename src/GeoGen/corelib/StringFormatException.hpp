#pragma once

#include <string>

#include "..\CodeLocation.hpp"
#include "..\ErrorCode.hpp"
#include "..\runtime\RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class StringFormatException : public runtime::RuntimeException
		{
		private:
			std::string keyStringValue;
		public:
			StringFormatException(CodeLocation location) :
				RuntimeException(GGE2701_IncorrectStringFormat, location), keyStringValue(keyStringValue) {};

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Invalid template string format on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}