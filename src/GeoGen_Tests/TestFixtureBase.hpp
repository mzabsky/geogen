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

#include <map>

#include <GeoGen/GeoGen.hpp>
#include "NoExceptionException.hpp"

#include "../png++/png.hpp"

using namespace std;
using namespace geogen;
using namespace compiler;
using namespace runtime;
using namespace corelib;
using namespace testlib;
using namespace genlib;
using namespace renderer;
using namespace random;

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

	static void SaveRenders(String testName, RenderedMapTable const& renderedMapTable)
	{
		for (RenderedMapTable::const_iterator it = renderedMapTable.Begin(); it != renderedMapTable.End(); it++)
		{
			png::image< png::ga_pixel_16 > image(it->second->GetRectangle().GetSize().GetWidth(), it->second->GetRectangle().GetSize().GetHeight());
			for (size_t y = 0; y < image.get_height(); ++y)
			{
				for (size_t x = 0; x < image.get_width(); ++x)
				{
					image[y][x] = png::ga_pixel_16((unsigned short)((long)-HEIGHT_MIN + (long)(*it->second)(x, y)));
				}
			}

			stringstream ss;
			ss << StringToAscii(testName) << "_" << StringToAscii(it->first) << "_" << it->second->GetRectangle().GetPosition().GetX() << "_" << it->second->GetRectangle().GetPosition().GetY() << ".png";
			image.write(ss.str());
		}
	}

protected:
	static auto_ptr<CompiledScript>  TestGetCompiledScript(String const& script)
	{
		Configuration configuration;
		configuration.MainMapIsMandatory = false;

		Compiler compiler(configuration);
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

	static void TestScript(String script)
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript(script);

		VirtualMachine vm(*compiledScript.get(), compiledScript->CreateScriptParameters());

		vm.Run();

		if (vm.GetStatus() != VIRTUAL_MACHINE_STATUS_FINISHED)
		{
			throw exception("VM finished in incorrect status.");
		}
	}

	static void TestScript(String script, ScriptParameters const& arguments)
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