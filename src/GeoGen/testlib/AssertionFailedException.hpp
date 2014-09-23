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

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Assertion failed: expected " << expected << ", got " << actual << " on line " << location.GetLine() << ", column " << location.GetColumn() << ".";

				return ss.str();
			}
		};
	}
}