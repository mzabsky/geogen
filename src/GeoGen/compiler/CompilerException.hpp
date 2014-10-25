#pragma once

#include "../CodeLocation.hpp"
#include "../GeoGenException.hpp"

namespace geogen 
{
	namespace compiler 
	{
		/// Base class for exceptions throw during script compilation.
		class CompilerException : public GeoGenException
		{
		private:
			CodeLocation location;
		public:

			/// Constructor.
			/// @param code The error code.
			/// @param location The location.
			explicit CompilerException(ErrorCode code, CodeLocation location) :
				GeoGenException(code), location(location) {};

			/// Gets the location.
			/// @return The location.
			inline CodeLocation GetLocation(){ return this->location; }
		};
	}
}