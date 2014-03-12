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
			GeoGenException(GGE4000_ApiUsageError) {};

		virtual std::string GetDetailMessage()
		{
			return "Incorrect GeoGen API usage dectected (" + message + ").";
		}
	};
}