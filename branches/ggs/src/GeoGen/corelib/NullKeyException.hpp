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
		class NullKeyException : public runtime::RuntimeException
		{
		private:
		public:
			NullKeyException(CodeLocation location) :
				RuntimeException(GGE2603_ArrayNullKey, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to insert null key into an array on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}