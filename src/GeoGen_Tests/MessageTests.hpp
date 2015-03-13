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

#include "TestFixtureBase.hpp"

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


	static void TestPrintWithoutArgsHandler(VirtualMachine* virtualMachine, CodeLocation location, String const& formattedMessage, String const& unformattedMessage, std::vector<String> arguments)
	{
		ASSERT_EQUALS(String, GG_STR("Text message!"), formattedMessage);
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

	static void TestPrintWithArgsHandler(VirtualMachine* virtualMachine, CodeLocation location, String const& formattedMessage, String const& unformattedMessage, std::vector<String> arguments)
	{
		ASSERT_EQUALS(String, GG_STR("1 2"), formattedMessage);
		ASSERT_EQUALS(String, GG_STR("{0} {1}"), unformattedMessage);
		ASSERT_EQUALS(unsigned, 2, arguments.size());
		ASSERT_EQUALS(String, "1", arguments[0]);
		ASSERT_EQUALS(String, "2", arguments[1]);
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
};;