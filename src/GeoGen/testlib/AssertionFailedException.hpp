#pragma once

#include <string>
#include <iostream>
#include <stdexcept>

#include "../ErrorCode.hpp"
#include "../GeoGenException.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	class AssertionFailedException : public GeoGenException
	{
	private:
		CodeLocation location;
		std::string expected;
		std::string actual;
	public:
		explicit AssertionFailedException(CodeLocation location, std::string const& expected, std::string const& actual) :
			GeoGenException((ErrorCode)9000), location(location), expected(expected), actual(actual) {};

		virtual std::string GetDetailMessage()
		{
			std::stringstream ss;
			ss << "Assertion failed: epected " << expected << ", got " << actual << " on line " << location.GetLine() << ", column " << location.GetColumn() << "."; 

			return ss.str();
		}
	};
}