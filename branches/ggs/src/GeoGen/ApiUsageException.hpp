#pragma once

#include "ErrorCode.hpp"
#include "GeoGenException.hpp"

namespace geogen
{
	/// Exception thrown when the API detects incorrect usage (such as incorrect parameter or a method being called in incorrect context).
	class ApiUsageException : public GeoGenException
	{
	private:
		String message;
	public:

		/// Constructor.
		/// @param message The message.
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