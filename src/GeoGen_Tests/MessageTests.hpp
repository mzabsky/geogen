#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\UserErrorException.hpp"

class MessageTests : public TestFixtureBase
{
	static bool messageTriggered;
public:
	static void TestTriggerErrorWithoutArgs()
	{
		TEST_SCRIPT_FAILURE(UserErrorException, "\n\
			TriggerError(\"Text error!\");\n\
		");
	}

	static void TestTriggerErrorWithArgs()
	{
		TEST_SCRIPT_FAILURE(UserErrorException, "\n\
			TriggerError(\"Text error!\", 6, \"aaa\");\n\
		");
	}


	static void TestPrintWithoutArgsHandler(VirtualMachine* virtualMachine, CodeLocation location, String const& message)
	{
		ASSERT_EQUALS(string, "Text message!", message);
		messageTriggered = true;
	}

	static void TestPrintWithoutArgs()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			Print(\"Text message!\");\n\
		");

		messageTriggered = false;

		VirtualMachine vm(*compiledScript.get(), ScriptParameters());
		vm.SetScriptMessageHandler(TestPrintWithoutArgsHandler);
		vm.Run();

		ASSERT_EQUALS(bool, true, messageTriggered);
	}

	static void TestPrintWithArgsHandler(VirtualMachine* virtualMachine, CodeLocation location, std::string const& message)
	{
		ASSERT_EQUALS(string, "1 2", message);
		messageTriggered = true;
	}

	static void TestPrintWithArgs()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			Print(\"{0} {1}\", 1, \"2\");\n\
		");

		messageTriggered = false;

		VirtualMachine vm(*compiledScript.get(), ScriptParameters());
		vm.SetScriptMessageHandler(TestPrintWithArgsHandler);
		vm.Run();

		ASSERT_EQUALS(bool, true, messageTriggered);
	}

	MessageTests() : TestFixtureBase("MessageTests")
	{
		ADD_TESTCASE(TestTriggerErrorWithoutArgs);
		ADD_TESTCASE(TestTriggerErrorWithArgs);
		ADD_TESTCASE(TestPrintWithoutArgs);
		ADD_TESTCASE(TestPrintWithArgs);
	}
};