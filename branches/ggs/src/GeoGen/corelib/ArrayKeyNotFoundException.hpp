#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayKeyNotFoundException : public runtime::RuntimeException
		{
		private:
			String keyStringValue;
		public:
			ArrayKeyNotFoundException(CodeLocation location, String const& keyStringValue) :
				RuntimeException(GGE2602_ArrayKeyNotFound, location), keyStringValue(keyStringValue) {};

			inline String GetKeyStringValue()
			{
				return this->keyStringValue;
			}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Entry with key \"" << this->GetKeyStringValue() << "\" was not found in an array on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}