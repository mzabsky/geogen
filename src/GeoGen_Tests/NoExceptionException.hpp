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

#include <GeoGen/GeoGen.hpp>

class NoExceptionException : public geogen::GeoGenException
{
private:
	geogen::String exceptionType;
public:
	NoExceptionException(geogen::String exceptionType) :
		GeoGenException((geogen::ErrorCode)9000), exceptionType(exceptionType) {};

	virtual ~NoExceptionException() throw () {}

	virtual geogen::String GetDetailMessage()
	{
		geogen::StringStream ss;
		ss << "Exception of expected type " << exceptionType << " was not thrown.";

		return ss.str();
	}
};