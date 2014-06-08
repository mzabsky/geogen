#pragma once

#include "../GeoGen/ErrorCode.hpp"
#include "../GeoGen/GeoGenException.hpp"

class NoExceptionException : public geogen::GeoGenException
{
private:
	geogen::String exceptionType;
public:
	NoExceptionException(geogen::String exceptionType) :
		GeoGenException((geogen::ErrorCode)9000), exceptionType(exceptionType) {};

	virtual geogen::String GetDetailMessage()
	{
		geogen::StringStream ss;
		ss << "Exception of expected type " << exceptionType << " was not thrown.";

		return ss.str();
	}
};