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
		class UnknownRelativeCoordinateDirectionException : public runtime::RuntimeException
		{
		private:
		public:
			UnknownRelativeCoordinateDirectionException(CodeLocation location) :
				RuntimeException(GGE2703_UnknownRelativeCoordinateDirection, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to use relative coordinate without specifying its direction on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}