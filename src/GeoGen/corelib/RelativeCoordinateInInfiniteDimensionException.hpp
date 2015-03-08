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
		class RelativeCoordinateInInfiniteDimensionException : public runtime::RuntimeException
		{
		private:
		public:
			RelativeCoordinateInInfiniteDimensionException(CodeLocation location) :
				RuntimeException(GGE2704_RelativeCoordinateInInfiniteDimensions, location) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to use relative coordinate in dimension in which the map is infinite on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}