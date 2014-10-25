#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class InvalidScaleException : public runtime::RuntimeException
		{
		private:
		public:
			InvalidScaleException(CodeLocation location) :
				RuntimeException(GGE2714_InvalidScale, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Scale was invalid on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ". Scale must be greater than 0.1 and less than 10.";
				return ss.str();
			}
		};
	}
}