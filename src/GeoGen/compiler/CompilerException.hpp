#pragma once

#include <string>
#include <stdexcept>

#include "../GeoGenException.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class CompilerException : public GeoGenException
		{
		public:
			explicit CompilerException(ErrorCode code) :
				GeoGenException(code) {};
		};
	}
}