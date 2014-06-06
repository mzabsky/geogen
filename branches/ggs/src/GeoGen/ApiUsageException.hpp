#pragma once

#include "ErrorCode.hpp"
#include "GeoGenException.hpp"

namespace geogen
{
	class ApiUsageException : public GeoGenException
	{
	private:
		String message;
	public:
		explicit ApiUsageException(String const& message) :
			GeoGenException(GGE3000_ApiUsageError), message(message) {};

		virtual String GetDetailMessage()
		{
			StringStream ss;
			ss << "Incorrect GeoGen API usage dectected (" << message << ").";

			return ss.str();
		};
	};
}