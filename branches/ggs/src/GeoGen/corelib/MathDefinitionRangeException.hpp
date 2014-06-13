#pragma once

#include "../String.hpp"
#include "../Number.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime\RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class MathDefinitionRangeException : public runtime::RuntimeException
		{
		private:
			String operationName;
			Number number;
		public:
			MathDefinitionRangeException(ErrorCode errorCode, CodeLocation location, String const& operationName, Number number) :
				RuntimeException(errorCode, location), operationName(operationName), number(number) {};

			inline Number GetNumber()
			{
				return this->number;
			}

			inline String GetOperationName()
			{
				return this->operationName;
			}


			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "\"" << this->GetNumber() << "\" is outside of the definition range of operation \"" << this->GetOperationName() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}