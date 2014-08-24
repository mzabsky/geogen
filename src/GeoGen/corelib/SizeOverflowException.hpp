#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class SizeOverflowException : public runtime::RuntimeException
		{
		private:
		public:
			SizeOverflowException(CodeLocation location) :
				RuntimeException(GGE2309_SizeOverflow, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to use too small/large size/distance value on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}