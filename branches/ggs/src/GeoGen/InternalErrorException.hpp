#pragma once

#include "ErrorCode.hpp"
#include "GeoGenException.hpp"

namespace geogen
{
	/// Exception thrown when an unrecoverable internal error in GeoGen occurs. These should be reported as bugs (although an incorrect usage of the API might also be sometimes the cause).
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