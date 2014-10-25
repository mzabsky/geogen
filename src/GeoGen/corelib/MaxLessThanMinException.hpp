#pragma once

#include "../String.hpp"
#include "../Number.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class MaxLessThanMinException : public runtime::RuntimeException
		{
		private:
		public:
			MaxLessThanMinException(CodeLocation location) :
				RuntimeException(GGE2705_MaxLessThanMin, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Maximum was less than minimum on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}