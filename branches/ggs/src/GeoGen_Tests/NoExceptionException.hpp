#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

#include "../GeoGen/ErrorCode.hpp"
#include "../GeoGen/GeoGenException.hpp"

class NoExceptionException : public geogen::GeoGenException
{
private:
	std::string exceptionType;
public:
	NoExceptionException(std::string exceptionType) :
		GeoGenException((geogen::ErrorCode)9000), exceptionType(exceptionType) {};

	virtual std::string GetDetailMessage()
	{
		std::stringstream ss;
		ss << "Exception of expected type " << exceptionType << " was not thrown.";

		return ss.str();
	}
};