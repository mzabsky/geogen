#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\UserErrorException.hpp"

class MessageTests : public TestFixtureBase
{
public:
	static void TestTriggerError()
	{
		TEST_SCRIPT_FAILURE(UserErrorException, "\n\
			TriggerError(\"Text error!\");\n\
		");
	}

	MessageTests() : TestFixtureBase("MessageTests")
	{
		ADD_TESTCASE(TestTriggerError);
	}
};