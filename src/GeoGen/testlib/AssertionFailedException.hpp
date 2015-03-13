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

#include "../GeoGenException.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace testlib
	{
		/// Exception thrown when an assertion in TestLibrary fails.
		class AssertionFailedException : public GeoGenException
		{
		private:
			CodeLocation location;
			String expected;
			String actual;
		public:

			/// Constructor.
			/// @param location The code location.
			/// @param expected The expected value.
			/// @param actual The actual value.
			explicit AssertionFailedException(CodeLocation location, String const& expected, String const& actual) :
				GeoGenException((ErrorCode)9000), location(location), expected(expected), actual(actual) {};

			virtual ~AssertionFailedException() throw () {}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Assertion failed: expected " << expected << ", got " << actual << " on line " << location.GetLine() << ", column " << location.GetColumn() << ".";

				return ss.str();
			}
		};
	}
}