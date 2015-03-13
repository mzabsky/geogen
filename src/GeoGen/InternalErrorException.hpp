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
			GeoGenException(GGE5000_InternalError), message(message) {};

		virtual ~InternalErrorException() throw () {}

		virtual String GetDetailMessage()
		{
			StringStream ss;
			ss << "Internal error has occured in GeoGen (" << message << ").";

			return ss.str();
		}
	};
}