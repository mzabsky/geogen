#pragma once

#include "../CodeLocation.hpp"
#include "../GeoGenException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Base class for exceptions throw during script execution.
		class RuntimeException : public GeoGenException
		{
		private:
			CodeLocation location;
		public:

			/// Constructor.
			/// @param code The error code.
			/// @param location The code location.
			explicit RuntimeException(ErrorCode code, CodeLocation location) :
				GeoGenException(code), location(location) {};

			/// Gets the code location.
			/// @return The code location.
			inline CodeLocation GetLocation(){ return this->location; }

			virtual String GetDetailMessage() = 0;
		};
	}
}