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
	/// Exception thrown when the API detects incorrect usage (such as incorrect parameter or a method being called in incorrect context).
	class ApiUsageException : public GeoGenException
	{
	private:
		String message;
	public:
		/// Constructor.
		/// @param message The message.
		explicit ApiUsageException(String const& message) :
			GeoGenException(GGE4000_ApiUsageError), message(message) {};

		virtual ~ApiUsageException() throw () {}

		virtual String GetDetailMessage()
		{
			StringStream ss;
			ss << "Incorrect GeoGen API usage dectected (" << message << ").";

			return ss.str();
		};
	};
}