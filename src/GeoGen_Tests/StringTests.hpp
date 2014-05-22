#pragma once

#include "TestFixtureBase.hpp"

class StringTests : public TestFixtureBase
{
public:
	static void TestStringLiteralWithEscapeSequencesMessageHandler(VirtualMachine* virtualMachine, CodeLocation location, std::string const& message)
	{
		ASSERT_EQUALS(string, "A\nA", message);
	}

	static void TestStringLiteralWithEscapeSequences()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			Print(\"A\\nA!\");\n\
		");

		VirtualMachine vm(*compiledScript.get(), ScriptParameters());
		vm.SetScriptMessageHandler(TestStringLiteralWithEscapeSequencesMessageHandler);
		vm.Run();
	}

	StringTests() : TestFixtureBase("StringTests")
	{
		ADD_TESTCASE(TestStringLiteralWithEscapeSequences);
	}
};