#pragma once

#include <map>
#include <string>
#include <iostream>

#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/testlib/TestLibrary.hpp"
#include "NoExceptionException.hpp"
#include "..\GeoGen\testlib\AssertionFailedException.hpp"

using namespace std;
using namespace geogen;
using namespace compiler;
using namespace runtime;
using namespace corelib;
using namespace testlib;

#define ADD_TESTCASE(tc) this->AddTestCase(#tc, tc);

#define TEST_SCRIPT_FAILURE(exception, script) TestScriptFailure<exception>(script, #exception)

#define RUN_FIXTURE(fixture) fixture().Run(numberOfFailures, numberOfPassed);

#define ASSERT_EQUALS(t, expected, actual) AssertEquals<t>(expected, actual, __LINE__)

class TestFixtureBase
{	
public:
	typedef void(*TestCase)();

private:
	string name;
	map<string, TestCase> testCases;
	static TestLibrary testLib;
protected:
	void AddTestCase(string name, TestCase testCase)
	{
		this->testCases.insert(std::pair<string, TestCase>(name, testCase));
	}

	TestFixtureBase(string name) : name(name)
	{
	}
public:
	void Run(int& numberOfFailures, int& numberOfPassed)
	{
		for (map<string, TestCase>::iterator it = this->testCases.begin(); it != this->testCases.end(); it++)
		{
			try {
				it->second();
			}
			catch (GeoGenException& e)
			{
				cout << "Test case " << name << "::" << it->first << " failed with error  \"GGE" << e.GetErrorCode() << ": " << e.GetDetailMessage() << "\"." << endl;

				numberOfFailures++;

				continue;
			}
			catch (exception e){
				cout << "Test case " << name << "::" << it->first << " failed with message \"" << e.what() << "\"." << endl;

				numberOfFailures++;

				continue;
			}

			cout << "Test case " << name << "::" << it->first << " passed." << endl;

			numberOfPassed++;
		}
	}

protected:
	static auto_ptr<CompiledScript>  TestGetCompiledScript(std::string const& script)
	{
		Compiler compiler;
		auto_ptr<CompiledScript> compiledScript = auto_ptr<CompiledScript>(compiler.CompileScript(script));
		compiledScript->AddLibrary(&testLib);

		return compiledScript;
	}

	static void TestScript(string script, ScriptParameters const& arguments = ScriptParameters())
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript(script);


		VirtualMachine vm(*compiledScript.get(), arguments);

		vm.Run();

		if (vm.GetStatus() != VIRTUAL_MACHINE_STATUS_FINISHED)
		{
			throw exception("VM finished in incorrect status.");
		}
	}

	template<typename TException>
	static void TestScriptFailure(string script, string exceptionName)
	{
		try{
			TestScript(script);
		}
		catch (TException&){
			return;
		}

		throw NoExceptionException(exceptionName);
	}

	template<typename T>
	static void AssertEquals(T a, T b, int line)
	{
		stringstream ssa;
		ssa << a;
		stringstream ssb;
		ssb << b;

		if (a != b)
		{
			throw AssertionFailedException(CodeLocation(line, 0), ssa.str(), ssb.str());
		}
	}
};