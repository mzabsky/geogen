#pragma once

#include "TestFixtureBase.hpp"

class StringTests : public TestFixtureBase
{
public:
	static void TestStringLiteralWithEscapeSequencesMessageHandler(VirtualMachine* virtualMachine, CodeLocation location, String const& message)
	{
		ASSERT_EQUALS(String, GG_STR("Text \n \t \\ \"!"), message);
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

	static void TestFormatString()
	{
		vector<String> args;
		args.push_back(GG_STR("0a"));
		args.push_back(GG_STR("1a"));
		args.push_back(GG_STR("2a"));
		args.push_back(GG_STR("3a"));
		args.push_back(GG_STR("4a"));
		args.push_back(GG_STR("5a"));
		args.push_back(GG_STR("6a"));
		args.push_back(GG_STR("7a"));
		args.push_back(GG_STR("8a"));
		args.push_back(GG_STR("9a"));
		args.push_back(GG_STR("10a"));
		args.push_back(GG_STR("11a"));
		args.push_back(GG_STR("12a"));
		args.push_back(GG_STR("13a"));

		String result = FormatString(GG_STR("{0} {1} {3} {2} {4} {5} {6} {7} {8} {9} {10} {11} {12} {12} {1} {{ }}"), args);

		ASSERT_EQUALS(String, GG_STR("0a 1a 3a 2a 4a 5a 6a 7a 8a 9a 10a 11a 12a 12a 1a { }"), result);
	}
	
	static void TestFormatStringCharInIndexFails()
	{
		vector<String> args;
		args.push_back(GG_STR("0a"));
		args.push_back(GG_STR("1a"));
		args.push_back(GG_STR("2a"));
		args.push_back(GG_STR("3a"));
		args.push_back(GG_STR("4a"));
		args.push_back(GG_STR("5a"));
		args.push_back(GG_STR("6a"));
		args.push_back(GG_STR("7a"));
		args.push_back(GG_STR("8a"));
		args.push_back(GG_STR("9a"));
		args.push_back(GG_STR("10a"));
		args.push_back(GG_STR("11a"));
		args.push_back(GG_STR("12a"));
		args.push_back(GG_STR("13a"));

		bool thrown = false;
		try
		{
			FormatString(GG_STR("{0} {1} {a} {2} {4} {5} {6} {7} {8} {9} {10} {11} {12} {12} {1} {{ }}"), args);
		}
		catch (ApiUsageException)
		{
			thrown = true;
		}

		ASSERT_EQUALS(bool, true, thrown);
	}

	static void TestFormatStringUnexpectedClosingBracketFails()
	{
		vector<String> args;
		args.push_back(GG_STR("0a"));
		args.push_back(GG_STR("1a"));
		args.push_back(GG_STR("2a"));
		args.push_back(GG_STR("3a"));
		args.push_back(GG_STR("4a"));
		args.push_back(GG_STR("5a"));
		args.push_back(GG_STR("6a"));
		args.push_back(GG_STR("7a"));
		args.push_back(GG_STR("8a"));
		args.push_back(GG_STR("9a"));
		args.push_back(GG_STR("10a"));
		args.push_back(GG_STR("11a"));
		args.push_back(GG_STR("12a"));
		args.push_back(GG_STR("13a"));

		bool thrown = false;
		try
		{
			FormatString(GG_STR("{0} {1} ( {a} {2} {4} {5} {6} {7} {8} {9} {10} {11} {12} {12} {1} {{ }}"), args);
		}
		catch (ApiUsageException)
		{
			thrown = true;
		}

		ASSERT_EQUALS(bool, true, thrown);
	}

	static void TestFormatStringTooHighIndexFails()
	{
		vector<String> args;
		args.push_back(GG_STR("0a"));
		args.push_back(GG_STR("1a"));
		args.push_back(GG_STR("2a"));
		args.push_back(GG_STR("3a"));
		args.push_back(GG_STR("4a"));
		args.push_back(GG_STR("5a"));
		args.push_back(GG_STR("6a"));
		args.push_back(GG_STR("7a"));
		args.push_back(GG_STR("8a"));
		args.push_back(GG_STR("9a"));
		args.push_back(GG_STR("10a"));
		args.push_back(GG_STR("11a"));
		args.push_back(GG_STR("12a"));
		args.push_back(GG_STR("13a"));

		bool thrown = false;
		try
		{
			FormatString(GG_STR("{0} {1} {25} {a} {2} {4} {5} {6} {7} {8} {9} {10} {11} {12} {12} {1} {{ }}"), args);
		}
		catch (ApiUsageException)
		{
			thrown = true;
		}

		ASSERT_EQUALS(bool, true, thrown);
	}

#ifdef GEOGEN_WCHAR
	static void TestStringLiteralWithNonAsciiCharactersMessageHandler(VirtualMachine* virtualMachine, CodeLocation location, String const& message)
	{
		ASSERT_EQUALS(String, L"ěščřžýáíé", message);
	}

	static void TestStringLiteralWithNonAsciiCharacters()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript(L"\n\
			Print(\"ěščřžýáíé\");\n\
		");

		VirtualMachine vm(*compiledScript.get(), ScriptParameters());
		vm.SetScriptMessageHandler(TestStringLiteralWithNonAsciiCharactersMessageHandler);
		vm.Run();
	}
#endif

	StringTests() : TestFixtureBase("StringTests")
	{
		ADD_TESTCASE(TestStringLiteralWithEscapeSequences);
		ADD_TESTCASE(TestInvalidEscapeSequence);
		ADD_TESTCASE(TestMultilineString);
		ADD_TESTCASE(TestFormatString);
		ADD_TESTCASE(TestFormatStringCharInIndexFails);
		ADD_TESTCASE(TestFormatStringUnexpectedClosingBracketFails);
		ADD_TESTCASE(TestFormatStringTooHighIndexFails);

#ifdef GEOGEN_WCHAR
		ADD_TESTCASE(TestStringLiteralWithNonAsciiCharacters);
#endif
	}
};