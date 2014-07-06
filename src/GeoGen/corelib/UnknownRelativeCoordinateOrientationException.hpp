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
		class UnknownRelativeCoordinateOrientationException : public runtime::RuntimeException
		{
		private:
		public:
			UnknownRelativeCoordinateOrientationException(CodeLocation location) :
				RuntimeException(GGE2703_UnknownRelativeCoordinateOrientation, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to use relative coordinate without specifying its orientation on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}