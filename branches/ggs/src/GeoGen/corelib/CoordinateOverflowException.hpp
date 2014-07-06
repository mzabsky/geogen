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
		class CoordinateOverflowException : public runtime::RuntimeException
		{
		private:
		public:
			CoordinateOverflowException(CodeLocation location) :
				RuntimeException(GGE2304_CoordinateOverflow, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Too large or too small coordinate detected on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}