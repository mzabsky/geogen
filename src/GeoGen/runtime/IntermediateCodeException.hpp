
#pragma once

#include <string>
#include <stdexcept>

#include "../ErrorCode.hpp"
#include "../GeoGenException.hpp"

namespace geogen
{
	namespace runtime
	{
		class IntermediateCodeException : public GeoGenException
		{
		private:
			std::string message;
		public:
			explicit IntermediateCodeException(std::string const& message) :
				GeoGenException(GGE4001_IntermediateCodeError) {};

			virtual std::string GetDetailMessage()
			{
				return "Incorrect intermediate code was being executed, possibly indicating an internal error in the compiler (" + message + ").";
			}
		};
	}
}