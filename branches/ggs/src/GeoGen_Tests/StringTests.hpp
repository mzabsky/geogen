#pragma once

#include "TestFixtureBase.hpp"

class StringTests : public TestFixtureBase
{
public:
	static void TestStringLiteralWithEscapeSequencesMessageHandler(VirtualMachine* virtualMachine, CodeLocation location, std::string const& message)
	{
		ASSERT_EQUALS(string, "Text \n \t \\ \"!", message);
	}

	static void TestStringLiteralWithEscapeSequences()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			Print(\"Text \\n \\t \\\\ \\\"!\");\n\
		");

		VirtualMachine vm(*compiledScript.get(), ScriptParameters());
		vm.SetScriptMessageHandler(TestStringLiteralWithEscapeSequencesMessageHandler);
		vm.Run();
	}

	static void TestInvalidEscapeSequence()
	{
		TEST_SCRIPT_FAILURE(InvalidEscapeSequenceException, "Print(\"Text \\a\");");
	}

	static void TestMultilineString()
	{
		TestScript("var a = \"\n\
			\";");
	}

	StringTests() : TestFixtureBase("StringTests")
	{
		ADD_TESTCASE(TestStringLiteralWithEscapeSequences);
		ADD_TESTCASE(TestInvalidEscapeSequence);
		ADD_TESTCASE(TestMultilineString);
	}
};