#pragma once

#include <string>

#include "..\CodeLocation.hpp"
#include "..\ErrorCode.hpp"
#include "..\runtime\RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayKeyNotFoundException : public runtime::RuntimeException
		{
		private:
			std::string keyStringValue;
		public:
			ArrayKeyNotFoundException(CodeLocation location, std::string const& keyStringValue) :
				RuntimeException(GGE2602_ArrayKeyNotFound, location), keyStringValue(keyStringValue) {};

			inline std::string GetKeyStringValue()
			{
				return this->keyStringValue;
			}

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Entry with key \"" << this->GetKeyStringValue() + "\" was not found in an array on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}