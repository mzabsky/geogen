#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class InvalidNumberOfStrataException : public runtime::RuntimeException
		{
		private:
		public:
			InvalidNumberOfStrataException(CodeLocation location) :
				RuntimeException(GGE2716_InvalidNumberOfStrata, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Number of strata was invalid on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ". Number of strata must be greater than or equal to 1 and less than or equal to 100.";
				return ss.str();
			}
		};
	}
}