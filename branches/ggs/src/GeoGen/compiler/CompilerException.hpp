#pragma once

#include <string>
#include <stdexcept>

#include "../CodeLocation.hpp"
#include "../GeoGenException.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class CompilerException : public GeoGenException
		{
		private:
			CodeLocation location;
		public:
			explicit CompilerException(ErrorCode code, CodeLocation location) :
				GeoGenException(code), location(location) {};

			inline CodeLocation GetLocation(){ return this->location; }
		};
	}
}