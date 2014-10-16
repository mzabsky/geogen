#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class InvalidStrengthException : public runtime::RuntimeException
		{
		private:
		public:
			InvalidStrengthException(CodeLocation location) :
				RuntimeException(GGE2715_InvalidStrength, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Strength was invalid on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ". Strength must be greater than or equal to 0 and less than or equal to 1.";
				return ss.str();
			}
		};
	}
}