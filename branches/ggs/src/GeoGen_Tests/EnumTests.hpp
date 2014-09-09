#pragma once

#include "TestFixtureBase.hpp"

class EnumTests : public TestFixtureBase
{
public:
	static void TestScriptEnum()
	{
		string script = "\n\
			enum MyEnum \n\
			{\n\
				Item1,\n\
				Item2\n\
			}\n\
			\n\
			global a = MyEnum.Item2;\n\
		";

		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript(script);

		VirtualMachine vm(*compiledScript.get(), ScriptParameters());

		vm.Run();

		if (vm.GetStatus() != VIRTUAL_MACHINE_STATUS_FINISHED)
		{
			throw exception("VM finished in incorrect status.");
		}

		ASSERT_EQUALS(bool, true, vm.GetGlobalVariableTable().IsVariableDeclared(GG_STR("a")));

		ManagedObject* o = vm.GetGlobalVariableTable().GetVariable(GG_STR("a"))->GetValue();

		//ASSERT_EQUALS(string, "MyEnum", o->GetType()->GetName());
		//ASSERT_EQUALS(Number, 1, dynamic_cast<NumberObject*>(o)->GetValue());
	}

	static void TestScriptEnumWithManualValues()
	{
		string script = "\n\
			enum MyEnum \n\
			{\n\
				Item1,\n\
				Item2 = 5,\n\
				Item3\n\
			}\n\
			\n\
			global a1 = MyEnum.Item1;\n\
			global a2 = MyEnum.Item2;\n\
			global a3 = MyEnum.Item3;\n\
		";

		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript(script);

		VirtualMachine vm(*compiledScript.get(), ScriptParameters());

		vm.Run();

		if (vm.GetStatus() != VIRTUAL_MACHINE_STATUS_FINISHED)
		{
			throw exception("VM finished in incorrect status.");
		}

		ASSERT_EQUALS(bool, true, vm.GetGlobalVariableTable().IsVariableDeclared(GG_STR("a1")));
		ASSERT_EQUALS(bool, true, vm.GetGlobalVariableTable().IsVariableDeclared(GG_STR("a2")));
		ASSERT_EQUALS(bool, true, vm.GetGlobalVariableTable().IsVariableDeclared(GG_STR("a3")));

		ManagedObject* o1 = vm.GetGlobalVariableTable().GetVariable(GG_STR("a1"))->GetValue();
		ManagedObject* o2 = vm.GetGlobalVariableTable().GetVariable(GG_STR("a2"))->GetValue();
		ManagedObject* o3 = vm.GetGlobalVariableTable().GetVariable(GG_STR("a3"))->GetValue();

		//ASSERT_EQUALS(string, "MyEnum", o->GetType()->GetName());
		ASSERT_EQUALS(Number, 0, dynamic_cast<NumberObject*>(o1)->GetValue());
		ASSERT_EQUALS(Number, 5, dynamic_cast<NumberObject*>(o2)->GetValue());
		ASSERT_EQUALS(Number, 6, dynamic_cast<NumberObject*>(o3)->GetValue());
	}

	static void TestScriptEnumWithNonIntegerValue()
	{
			TEST_SCRIPT_FAILURE(InvalidSymbolDefinitionException, "\n\
			enum MyEnum \n\
			{\n\
				Item1 = 0.3\n\
			}\n\
		");
	}

	static void TestScriptEnumWithValueRedefinition()
	{
		TEST_SCRIPT_FAILURE(SymbolRedefinitionException, "\n\
			enum MyEnum \n\
			{\n\
				Item1,\n\
				Item2,\n\
				Item2\n\
			}\n\
		");
	}

	static void TestEmptyScriptEnumFails()
	{
		TEST_SCRIPT_FAILURE(UnexpectedTokenException, "\n\
			enum MyEnum \n\
			{\n\
			}\n\
		");
	}

	static void TestFromNumber()
	{
		TestScript("\n\
			enum TestEnum\n\
			{\n\
				A = 1,\n\
				B = 2\n\
			}\n\
			AssertEquals(TestEnum.B, TestEnum.FromNumber(2));\n\
		");
	}

	static void TestFromNumberFailsWithInvalidNumber()
	{
		TEST_SCRIPT_FAILURE(corelib::InvalidEnumValueException, "\n\
			enum TestEnum\n\
			{\n\
				A = 1,\n\
				B = 2\n\
			}\n\
			AssertEquals(TestEnum.B, TestEnum.FromNumber(3));\n\
		");
	}

	static void TestFromEnum()
	{
		TestScript("\n\
			enum TestEnum\n\
			{\n\
				A = 1,\n\
				B = 2\n\
			}\n\
			AssertEquals(2, Number.FromEnum(TestEnum.B));\n\
		");
	}

	static void TestFromEnumFailsWithNonEnum()
	{
		TEST_SCRIPT_FAILURE(runtime::IncorrectTypeException, "\n\
			enum TestEnum\n\
			{\n\
				A = 1,\n\
				B = 2\n\
			}\n\
			AssertEquals(3, Number.FromEnum(2));\n\
		");
	}

	static void TestEnumWithReservedNameFails()
	{
		TEST_SCRIPT_FAILURE(compiler::InvalidSymbolDefinitionException, "\n\
			enum Type\n\
			{\n\
				A = 1,\n\
				B = 2\n\
			}\n\
		");
	}

	EnumTests() : TestFixtureBase("EnumTests")
	{
		ADD_TESTCASE(TestScriptEnum);
		/*ADD_TESTCASE(TestScriptEnumWithManualValues);*/
		ADD_TESTCASE(TestScriptEnumWithNonIntegerValue);
		ADD_TESTCASE(TestScriptEnumWithValueRedefinition);
		ADD_TESTCASE(TestEmptyScriptEnumFails);
		ADD_TESTCASE(TestFromNumber);
		ADD_TESTCASE(TestFromNumberFailsWithInvalidNumber);
		ADD_TESTCASE(TestFromEnum);
		ADD_TESTCASE(TestFromEnumFailsWithNonEnum);
		ADD_TESTCASE(TestEnumWithReservedNameFails);
	}
};