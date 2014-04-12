#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

#include "ErrorCode.hpp"
#include "GeoGenException.hpp"

namespace geogen
{
	class InternalErrorException : public GeoGenException
	{
	private:
		std::string message;
	public:
		explicit InternalErrorException(std::string const& message) :
			GeoGenException(GGE4000_InternalError), message(message) {};

		virtual std::string GetDetailMessage()
		{
			std::stringstream ss;
			ss << "Internal error has occured in GeoGen (" << message << ").";

			return ss.str();
		}
	};
}