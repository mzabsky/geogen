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

		ASSERT_EQUALS(bool, true, vm.GetGlobalVariableTable().IsVariableDeclared("a"));

		ManagedObject* o = vm.GetGlobalVariableTable().GetVariable("a")->GetValue();

		//ASSERT_EQUALS(string, "MyEnum", o->GetType()->GetName());
		ASSERT_EQUALS(Number, 1, dynamic_cast<NumberObject*>(o)->GetValue());
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

		ASSERT_EQUALS(bool, true, vm.GetGlobalVariableTable().IsVariableDeclared("a1"));
		ASSERT_EQUALS(bool, true, vm.GetGlobalVariableTable().IsVariableDeclared("a2"));
		ASSERT_EQUALS(bool, true, vm.GetGlobalVariableTable().IsVariableDeclared("a3"));

		ManagedObject* o1 = vm.GetGlobalVariableTable().GetVariable("a1")->GetValue();
		ManagedObject* o2 = vm.GetGlobalVariableTable().GetVariable("a2")->GetValue();
		ManagedObject* o3 = vm.GetGlobalVariableTable().GetVariable("a3")->GetValue();

		//ASSERT_EQUALS(string, "MyEnum", o->GetType()->GetName());
		ASSERT_EQUALS(Number, 0, dynamic_cast<NumberObject*>(o1)->GetValue());
		ASSERT_EQUALS(Number, 5, dynamic_cast<NumberObject*>(o2)->GetValue());
		ASSERT_EQUALS(Number, 6, dynamic_cast<NumberObject*>(o3)->GetValue());
	}

	EnumTests() : TestFixtureBase("EnumTests")
	{
		ADD_TESTCASE(TestScriptEnum);
		ADD_TESTCASE(TestScriptEnumWithManualValues);
	}
};