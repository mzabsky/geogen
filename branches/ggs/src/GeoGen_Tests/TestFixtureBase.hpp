#pragma once

#include <map>
#include <string>
#include <iostream>

#include "../GeoGen/GeoGen.hpp"
#include "../GeoGen/testlib/TestLibrary.hpp"
#include "NoExceptionException.hpp"

using namespace std;
using namespace geogen;
using namespace compiler;
using namespace runtime;
using namespace corelib;
using namespace testlib;

#define ADD_TESTCASE(tc) this->AddTestCase(#tc, tc);

#define TEST_SCRIPT_FAILURE(exception, script) TestScriptFailure<exception>(script, #exception)

#define RUN_FIXTURE(fixture) fixture().Run(numberOfFailures, numberOfPassed);

class TestFixtureBase
{	
public:
	typedef void(*TestCase)();

private:
	string name;
	map<string, TestCase> testCases;

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
	static void TestScript(string script)
	{
		TestLibrary testLib;

		Compiler compiler;
		auto_ptr<CompiledScript> compiledScript = auto_ptr<CompiledScript>(compiler.CompileScript(script));
		compiledScript->AddLibrary(&testLib);

		VirtualMachine vm(*compiledScript.get());

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
};