#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE2302_NumberOverflow occurs.
		class NumberOverflowException : public RuntimeException
		{
		private:
		public:
			explicit NumberOverflowException(CodeLocation location) :
				RuntimeException(GGE2302_NumberOverflow, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Mathematical operation resulted in a number that was too great on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}