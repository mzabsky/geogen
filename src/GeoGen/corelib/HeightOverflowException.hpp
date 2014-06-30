#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightOverflowException : public runtime::RuntimeException
		{
		private:
		public:
			HeightOverflowException(CodeLocation location) :
				RuntimeException(GGE2303_HeightOverflow, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to use height greater than 1.0 or lesser than -1.0 on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}