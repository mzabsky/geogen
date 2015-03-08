#pragma once

#include "../String.hpp"
#include "../Number.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class InvalidEnumValueException : public runtime::RuntimeException
		{
		private:
			Number number;
			String typeName;
		public:
			InvalidEnumValueException(CodeLocation location, Number number, String const& typeName) :
				RuntimeException(GGE2702_InvalidEnumNumericValue, location), number(number), typeName(typeName) {};

			inline String const& GetTypeName()
			{
				return this->typeName;
			}

			inline Number GetNumber()
			{
				return this->number;
			}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Number \"" << this->GetNumber() << "\" does not correspond to an enum value of type \"" << this->GetTypeName()  << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}