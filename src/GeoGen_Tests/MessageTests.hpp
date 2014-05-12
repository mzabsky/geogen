#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\UserErrorException.hpp"

class MessageTests : public TestFixtureBase
{
	static bool messageTriggered;
public:
	static void TestTriggerError()
	{
		TEST_SCRIPT_FAILURE(UserErrorException, "\n\
			TriggerError(\"Text error!\");\n\
		");
	}

	static void TestScriptMessageHandler(VirtualMachine* virtualMachine, CodeLocation location, std::string const& message)
	{
		ASSERT_EQUALS(string, "Text message!", message);
		messageTriggered = true;
	}

	static void TestPrint()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			Print(\"Text message!\");\n\
		");

		messageTriggered = false;

		VirtualMachine vm(*compiledScript.get(), ScriptParameters());
		vm.SetScriptMessageHandler(TestScriptMessageHandler);
		vm.Run();

		ASSERT_EQUALS(bool, true, messageTriggered);
	}

	MessageTests() : TestFixtureBase("MessageTests")
	{
		ADD_TESTCASE(TestTriggerError);
		ADD_TESTCASE(TestPrint);
	}
};