#pragma once

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "../ErrorCode.hpp"
#include "../runtime/RuntimeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayIndexNotFoundException : public runtime::RuntimeException
		{
		private:
			int index;
		public:
			ArrayIndexNotFoundException(CodeLocation location, int index) :
				RuntimeException(GGE2604_ArrayIndexNotFound, location), index(index) {};

			inline int GetIndex()
			{
				return this->index;
			}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Key/value pair with index \"" << this->GetIndex() << "\" was not found in an array on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}