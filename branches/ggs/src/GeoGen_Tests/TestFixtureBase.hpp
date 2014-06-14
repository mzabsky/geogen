#pragma once

#include <map>

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

/*
#ifdef GEOGEN_WCHAR
	#define Cout wcout
#else
	#define Cout cout
#endif
*/

#define ADD_TESTCASE(tc) this->AddTestCase(#tc, tc);

#define TEST_SCRIPT_FAILURE(exception, script) TestScriptFailure<exception>(AnyStringToString(script), #exception)

#define RUN_FIXTURE(fixture) fixture().Run(numberOfFailures, numberOfPassed);

#define ASSERT_EQUALS(t, expected, actual) AssertEquals<t>(expected, actual, __LINE__)

class TestFixtureBase
{	
public:
	typedef void(*TestCase)();

private:
	String name;
	map<String, TestCase> testCases;
	static TestLibrary testLib;
protected:
	void AddTestCase(String name, TestCase testCase)
	{
		this->testCases.insert(std::pair<String, TestCase>(name, testCase));
	}

#ifdef GEOGEN_WCHAR
	void AddTestCase(string name, TestCase testCase)
	{
		this->testCases.insert(std::pair<String, TestCase>(AnyStringToString(name), testCase));
	}
#endif

	TestFixtureBase(String name) : name(name)
	{
	}

#ifdef GEOGEN_WCHAR
	TestFixtureBase(std::string name) : name(AnyStringToString(name))
	{
	}
#endif
public:
	void Run(int& numberOfFailures, int& numberOfPassed)
	{
		for (map<String, TestCase>::iterator it = this->testCases.begin(); it != this->testCases.end(); it++)
		{
			try {
				it->second();
			}
			catch (GeoGenException& e)
			{
				Cout << "Test case " << name << "::" << it->first << " failed with error  \"GGE" << e.GetErrorCode() << ": " << e.GetDetailMessage() << "\"." << endl;

				numberOfFailures++;

				continue;
			}
			catch (exception e){
				Cout << "Test case " << name << "::" << it->first << " failed with message \"" << e.what() << "\"." << endl;

				numberOfFailures++;

				continue;
			}

			Cout << "Test case " << name << "::" << it->first << " passed." << endl;

			numberOfPassed++;
		}
	}

protected:
	static auto_ptr<CompiledScript>  TestGetCompiledScript(String const& script)
	{
		Compiler compiler;
		auto_ptr<CompiledScript> compiledScript = auto_ptr<CompiledScript>(compiler.CompileScript(script));
		compiledScript->AddLibrary(&testLib);

		return compiledScript;
	}

#ifdef GEOGEN_WCHAR
	static auto_ptr<CompiledScript>  TestGetCompiledScript(string const& script)
	{
		return TestGetCompiledScript(AnyStringToString(script));
	}
#endif

	static void TestScript(String script, ScriptParameters const& arguments = ScriptParameters())
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript(script);

		VirtualMachine vm(*compiledScript.get(), arguments);

		vm.Run();

		if (vm.GetStatus() != VIRTUAL_MACHINE_STATUS_FINISHED)
		{
			throw exception("VM finished in incorrect status.");
		}
	}

#ifdef GEOGEN_ASCII
	static void TestScript(string script, ScriptParameters const& arguments = ScriptParameters())
	{
		TestScript(AnyStringToString(script), arguments);
	}
#endif

	template<typename TException>
	static void TestScriptFailure(String script, string exceptionName)
	{
		try{
			TestScript(script);
		}
		catch (TException&){
			return;
		}

		throw NoExceptionException(AnyStringToString(exceptionName));
	}

	template<typename T>
	static void AssertEquals(T a, T b, int line)
	{
		StringStream ssa;
		ssa << a;
		StringStream ssb;
		ssb << b;

		if (a != b)
		{
			throw AssertionFailedException(CodeLocation(line, 0), ssa.str(), ssb.str());
		}
	}
};