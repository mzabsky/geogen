#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class StringFormatException : public runtime::RuntimeException
		{
		private:
			String keyStringValue;
		public:
			StringFormatException(CodeLocation location) :
				RuntimeException(GGE2701_IncorrectStringFormat, location), keyStringValue(keyStringValue) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Invalid template string format on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}