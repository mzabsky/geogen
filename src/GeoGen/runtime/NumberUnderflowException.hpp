#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class NumberUnderflowException : public RuntimeException
		{
		private:
		public:
			explicit NumberUnderflowException(CodeLocation location) :
				RuntimeException(GGE2308_NumberUnderflow, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Mathematical operation resulted in a number that was too small on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}