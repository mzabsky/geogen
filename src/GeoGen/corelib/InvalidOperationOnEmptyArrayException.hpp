#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class InvalidOperationOnEmptyArrayException : public runtime::RuntimeException
		{
		private:
			String operationName;
		public:
			InvalidOperationOnEmptyArrayException(CodeLocation location, String const& operationName) :
				RuntimeException(GGE2601_InvalidOperationOnEmptyArray, location), operationName(operationName) {};

			inline String GetOperationName()
			{
				return this->operationName;
			}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to execute operation \"" << this->GetOperationName() + "\" on an empty array on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}