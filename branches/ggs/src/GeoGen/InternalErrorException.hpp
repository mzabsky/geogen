#pragma once

#include <string>
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
			GeoGenException(GGE4000_InternalError) {};

		virtual std::string GetDetailMessage()
		{
			return "Internal error has occured in GeoGen (" + message + ").";
		}
	};
}