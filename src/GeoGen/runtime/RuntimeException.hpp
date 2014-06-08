#pragma once

#include "../CodeLocation.hpp"
#include "../GeoGenException.hpp"

namespace geogen
{
	namespace runtime
	{
		class RuntimeException : public GeoGenException
		{
		private:
			CodeLocation location;
		public:
			explicit RuntimeException(ErrorCode code, CodeLocation location) :
				GeoGenException(code), location(location) {};

			inline CodeLocation GetLocation(){ return this->location; }

			virtual String GetDetailMessage() = 0;
		};
	}
}