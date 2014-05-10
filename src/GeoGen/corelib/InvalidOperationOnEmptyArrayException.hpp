#pragma once

#include <string>

#include "..\CodeLocation.hpp"
#include "..\ErrorCode.hpp"
#include "..\runtime\RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class InvalidOperationOnEmptyArrayException : public runtime::RuntimeException
		{
		private:
			std::string operationName;
		public:
			InvalidOperationOnEmptyArrayException(CodeLocation location, std::string const& operationName) :
				RuntimeException(GGE2601_InvalidOperationOnEmptyArray, location), operationName(operationName) {};

			inline std::string GetOperationName()
			{
				return this->operationName;
			}

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Attempted to execute operation \"" << this->GetOperationName() + "\" on an empty array on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}