#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class NullReferenceException : public RuntimeException
		{
		public:
			NullReferenceException(CodeLocation location) :
				RuntimeException(GGE2106_NullReference, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to dereference null object reference on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}