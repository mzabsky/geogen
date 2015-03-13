/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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

		virtual ~GeoGenException() throw () {}

		virtual const char* what() const throw()
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