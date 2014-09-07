#pragma once

#include "../GeoGenException.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace testlib
	{
		class AssertionFailedException : public GeoGenException
		{
		private:
			CodeLocation location;
			String expected;
			String actual;
		public:
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