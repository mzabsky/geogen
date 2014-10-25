#pragma once

#include <stdexcept>

#include "String.hpp"
#include "ErrorCode.hpp"

namespace geogen
{
	/// Base class for all exceptions thrown by GeoGen.
	class GeoGenException : std::exception
	{
	private:
		ErrorCode code;
		std::string message; // This has to be narrow string, because std::exception uses narrow string in what() method. 
	public:
		explicit GeoGenException(ErrorCode code) : code(code)
		{
			std::stringstream ss;
			ss << "GeoGen error GGE" << (int)code << ". See GetDetailMessage for additional information." << std::endl;

			message = ss.str();
		}

		virtual const char* what() const
		{
			return this->message.c_str();
		}

		/// Gets an error code representing the error.
		/// @return The error code.
		ErrorCode GetErrorCode() const
		{
			return this->code;
		}

		/// Gets a detailed message of the exception.
		/// @return The detail message.
		virtual String GetDetailMessage() { return GG_STR(""); }
	};
}