#pragma once

#include "ErrorCode.hpp"
#include "GeoGenException.hpp"

namespace geogen
{
	class InternalErrorException : public GeoGenException
	{
	private:
		String message;
	public:
		explicit InternalErrorException(String const& message) :
			GeoGenException(GGE4000_InternalError), message(message) {};

		virtual String GetDetailMessage()
		{
			StringStream ss;
			ss << "Internal error has occured in GeoGen (" << message << ").";

			return ss.str();
		}
	};
}