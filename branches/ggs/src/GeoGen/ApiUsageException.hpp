#pragma once

#include <string>
#include <stdexcept>

#include "ErrorCode.hpp"
#include "GeoGenException.hpp"

namespace geogen
{
	class ApiUsageException : public GeoGenException
	{
	private:
		std::string message;
	public:
		explicit ApiUsageException(std::string const& message) :
			GeoGenException(GGE3000_ApiUsageError), message(message) {};

		virtual std::string GetDetailMessage()
		{
			std::stringstream ss;
			ss << "Incorrect GeoGen API usage dectected (" << message << ").";

			return ss.str();
		};
	};
}