#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\VariableRedefinitionException.hpp"
#include "..\GeoGen\runtime\BooleanScriptParameter.hpp"
#include "..\GeoGen\runtime\NumberScriptParameter.hpp"
#include "..\GeoGen\compiler\IncorrectScriptParameterAttributeTypeException.hpp"
#include "..\GeoGen\compiler\IncorrectMapSizeNumericValueException.hpp"
#include "..\GeoGen\compiler\MinGreaterThanMaxSizeException.hpp"

class MetadataTests : public TestFixtureBase
{
private:


public:
	static void TestGetStringMetadataValue()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				ValueName: \"ValueValue\"\n\
				\n\
			}\n\
		");

		MetadataValue const* value = compiledScript->GetMetadata().GetItem("ValueName");
		ASSERT_EQUALS(MetadataType, METADATA_TYPE_STRING, value->GetType());
		ASSERT_EQUALS(string, "ValueValue", dynamic_cast<MetadataString const*>(value)->GetValue());
	}

	static void TestGetIdentifierMetadataValue()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				ValueName: ValueValue \n\
				\n\
			}\n\
		");

		MetadataValue const* value = compiledScript->GetMetadata().GetItem("ValueName");
		ASSERT_EQUALS(MetadataType, METADATA_TYPE_IDENTIFIER, value->GetType());
		ASSERT_EQUALS(string, "ValueValue", dynamic_cast<MetadataIdentifier const*>(value)->GetValue());
	}

	static void TestGetNumberMetadataValue()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				ValueName: 5.5\n\
				\n\
			}\n\
		");

		MetadataValue const* value = compiledScript->GetMetadata().GetItem("ValueName");
		ASSERT_EQUALS(MetadataType, METADATA_TYPE_NUMBER, value->GetType());
		ASSERT_EQUALS(Number, 5.5, dynamic_cast<MetadataNumber const*>(value)->GetValue());
	}

	static void TestGetBooleanMetadataValue()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				ValueName: true\n\
				\n\
			}\n\
		");

		MetadataValue const* value = compiledScript->GetMetadata().GetItem("ValueName");
		ASSERT_EQUALS(MetadataType, METADATA_TYPE_BOOLEAN, value->GetType());
		ASSERT_EQUALS(bool, true, dynamic_cast<MetadataBoolean const*>(value)->GetValue());
	}

	static void TestGetSimpleCollectionMetadataValue()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				ValueName: { \"ValueValue\", 5.5, true }\n\
				\n\
			}\n\
		");

		MetadataValue const* value = compiledScript->GetMetadata().GetItem("ValueName");
		ASSERT_EQUALS(MetadataType, METADATA_TYPE_SIMPLE_COLLECTION, value->GetType());

		MetadataSimpleCollection const* collection = dynamic_cast<MetadataSimpleCollection const*>(value);
		ASSERT_EQUALS(size_t, 3, collection->Size());

		ASSERT_EQUALS(MetadataType, METADATA_TYPE_STRING, (*collection->Begin())->GetType());
		ASSERT_EQUALS(string, "ValueValue", dynamic_cast<MetadataString const*>(*collection->Begin())->GetValue());

		ASSERT_EQUALS(MetadataType, METADATA_TYPE_NUMBER, (*(collection->Begin() + 1))->GetType());
		ASSERT_EQUALS(Number, 5.5, dynamic_cast<MetadataNumber const*>(*(collection->Begin() + 1))->GetValue());

		ASSERT_EQUALS(MetadataType, METADATA_TYPE_BOOLEAN, (*(collection->Begin() + 2))->GetType());
		ASSERT_EQUALS(bool, true, dynamic_cast<MetadataBoolean const*>(*(collection->Begin() + 2))->GetValue());
	}

	static void TestGetKeyValueCollectionMetadataValue()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				ValueName: { StringKey: \"ValueValue\", NumberKey: 5.5, BooleanKey: true }\n\
				\n\
			}\n\
		");

		MetadataValue const* value = compiledScript->GetMetadata().GetItem("ValueName");
		ASSERT_EQUALS(MetadataType, METADATA_TYPE_KEYVALUE_COLLECTION, value->GetType());

		MetadataKeyValueCollection const* collection = dynamic_cast<MetadataKeyValueCollection const*>(value);
		ASSERT_EQUALS(size_t, 3, collection->Size());

		ASSERT_EQUALS(MetadataType, METADATA_TYPE_STRING, collection->GetItem("StringKey")->GetType());
		ASSERT_EQUALS(string, "ValueValue", dynamic_cast<MetadataString const*>(collection->GetItem("StringKey"))->GetValue());

		ASSERT_EQUALS(MetadataType, METADATA_TYPE_NUMBER, collection->GetItem("NumberKey")->GetType());
		ASSERT_EQUALS(Number, 5.5, dynamic_cast<MetadataNumber const*>(collection->GetItem("NumberKey"))->GetValue());

		ASSERT_EQUALS(MetadataType, METADATA_TYPE_BOOLEAN, collection->GetItem("BooleanKey")->GetType());
		ASSERT_EQUALS(bool, true, dynamic_cast<MetadataBoolean const*>(collection->GetItem("BooleanKey"))->GetValue());
	}

	static void TestMetadataKeyRedefinition()
	{
		TEST_SCRIPT_FAILURE(MetadataKeyRedefinitionException, "\n\
			metadata {\n\
				Parameters: \n\
				{ \n\
					BoolParam: { Type: Boolean },\n\
					BoolParam: { Type: Boolean }\n\
				}\n\
			}\n\
		");
	}

	static void TestParseBooleanScriptParameter()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Parameters: \n\
				{ \n\
					BoolParam: { Type: Boolean, Label: \"Boolean Parameter\", Description: \"Desc\", Default: true }\n\
				}\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();

		ASSERT_EQUALS(bool, true, params.ContainsItem("BoolParam"));
		ASSERT_EQUALS(ScriptParameterType, SCRIPT_PARAMETER_TYPE_BOOLEAN, params.GetItem("BoolParam")->GetType());		

		BooleanScriptParameter* param = dynamic_cast<BooleanScriptParameter*>(params.GetItem("BoolParam"));
		ASSERT_EQUALS(string, "Boolean Parameter", param->GetLabel());
		ASSERT_EQUALS(string, "Desc", param->GetDescription());
		ASSERT_EQUALS(bool, true, param->GetDefaultValue());
		ASSERT_EQUALS(bool, true, param->GetValue());
	}

	static void TestParseBooleanScriptParameterWithImplicitAttributes()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Parameters: \n\
				{ \n\
					BoolParam: { Type: Boolean }\n\
				}\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();

		BooleanScriptParameter* param = dynamic_cast<BooleanScriptParameter*>(params.GetItem("BoolParam"));
		ASSERT_EQUALS(string, "BoolParam", param->GetLabel());
		ASSERT_EQUALS(string, "", param->GetDescription());
		ASSERT_EQUALS(bool, false, param->GetDefaultValue());
		ASSERT_EQUALS(bool, false, param->GetValue());
	}

	static void TestParseNumberScriptParameter()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Parameters: \n\
				{ \n\
					NumberParam: { Type: Number, Label: \"Number Parameter\", Description: \"Desc\", Default: 5, Min: 2, Max: 10, Restriction: Integers }\n\
				}\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();

		ASSERT_EQUALS(bool, true, params.ContainsItem("NumberParam"));
		ASSERT_EQUALS(ScriptParameterType, SCRIPT_PARAMETER_TYPE_NUMBER, params.GetItem("NumberParam")->GetType());		

		NumberScriptParameter* param = dynamic_cast<NumberScriptParameter*>(params.GetItem("NumberParam"));
		ASSERT_EQUALS(string, "Number Parameter", param->GetLabel());
		ASSERT_EQUALS(string, "Desc", param->GetDescription());
		ASSERT_EQUALS(Number, 5, param->GetDefaultValue());
		ASSERT_EQUALS(Number, 5, param->GetValue());
		ASSERT_EQUALS(Number, 2, param->GetMin());
		ASSERT_EQUALS(Number, 10, param->GetMax());
		ASSERT_EQUALS(ScriptParameterValueRestriction, SCRIPT_PARAMETER_VALUE_RESTRICTION_INTEGERS, param->GetRestriction());
	}

	static void TestParseNumberScriptParameterWithImplicitAttributes()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Parameters: \n\
				{ \n\
					NumberParam: { Type: Number }\n\
				}\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();

		NumberScriptParameter* param = dynamic_cast<NumberScriptParameter*>(params.GetItem("NumberParam"));
		ASSERT_EQUALS(string, "NumberParam", param->GetLabel());
		ASSERT_EQUALS(string, "", param->GetDescription());
		ASSERT_EQUALS(Number, 0, param->GetDefaultValue());
		ASSERT_EQUALS(Number, 0, param->GetValue());
		ASSERT_EQUALS(Number, 0, param->GetMin());
		ASSERT_EQUALS(Number, 100, param->GetMax());
		ASSERT_EQUALS(ScriptParameterValueRestriction, SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED, param->GetRestriction());
	}

	static void TestUseBooleanScriptParameter()
	{
		ScriptParameters params;
		BooleanScriptParameter* param = new BooleanScriptParameter("BoolParam", "Boolean Parameter", "Desc", false);
		params.AddItem("BoolParam", param);
		param->SetValue(true);
		
		TestScript("\n\
			metadata {\n\
				Parameters: \n\
				{ \n\
					BoolParam: { Type: Boolean, Label: \"Boolean Parameter\", Description: \"Desc\", Default: false }\n\
				}\n\
			}\n\
			AssertEquals(true, Parameters.BoolParam);\n\
		", params);
	}

	static void TestUseBooleanScriptParameterWithDefaultValue()
	{
		TestScript("\n\
			metadata {\n\
				Parameters: \n\
				{ \n\
					BoolParam: { Type: Boolean, Label: \"Boolean Parameter\", Description: \"Desc\", Default: false }\n\
				}\n\
			}\n\
			AssertEquals(false, Parameters.BoolParam);\n\
		");
	}

	static void TestUseNumberScriptParameter()
	{
		ScriptParameters params;
		NumberScriptParameter* param = new NumberScriptParameter("NumberParam", "Number Parameter", "Desc", 2, 0, 10, SCRIPT_PARAMETER_VALUE_RESTRICTION_UNRESTRICTED);
		params.AddItem("NumberParam", param);
		param->SetValue(5);
		
		TestScript("\n\
			metadata {\n\
				Parameters: \n\
				{ \n\
					NumberParam: { Type: Number, Label: \"Number Parameter\", Description: \"Desc\", Default: 2, Max: 10 }\n\
				}\n\
			}\n\
			AssertEquals(5, Parameters.NumberParam);\n\
		", params);
	}

	static void TestUseNumberScriptParameterWithDefaultValue()
	{
		TestScript("\n\
			metadata {\n\
				Parameters: \n\
				{ \n\
					NumberParam: { Type: Number, Label: \"Number Parameter\", Description: \"Desc\", Default: 2 }\n\
				}\n\
			}\n\
			AssertEquals(2, Parameters.NumberParam);\n\
		");
	}

	static void TestDefaultInfiniteWidth()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, MAP_SIZE_INFINITE, params.GetDefaultMapWidth());
		ASSERT_EQUALS(unsigned, MAP_SIZE_MIN, params.GetMinMapWidth());
		ASSERT_EQUALS(unsigned, MAP_SIZE_INFINITE, params.GetMaxMapWidth());
	}

	static void TestExplicitInfiniteWidth()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Width: Infinite\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, MAP_SIZE_INFINITE, params.GetDefaultMapWidth());
		ASSERT_EQUALS(unsigned, MAP_SIZE_MIN, params.GetMinMapWidth());
		ASSERT_EQUALS(unsigned, MAP_SIZE_INFINITE, params.GetMaxMapWidth());
	}

	static void TestDefaultWidth()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Width: { Default: 1000 }\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, 1000, params.GetDefaultMapWidth());
	}

	static void TestDefaultWidthNonNumber()
	{
		TEST_SCRIPT_FAILURE(IncorrectScriptParameterAttributeTypeException, "\n\
			metadata {\n\
				Width: { Default: \"aaa\" }\n\
			}\n\
		");
	}

	static void TestDefaultWidthNonInteger()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Width: { Default: 1.5 }\n\
			}\n\
		");
	}

	static void TestDefaultWidthTooSmall()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Width: { Default: 0 }\n\
			}\n\
		");
	}

	static void TestDefaultWidthTooLarge()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Width: { Default: 10000000000000 }\n\
			}\n\
		");
	}

	static void TestMinWidth()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Width: { Min: 1000 }\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, 1000, params.GetMinMapWidth());
	}

	static void TestMinWidthNonNumber()
	{
		TEST_SCRIPT_FAILURE(IncorrectScriptParameterAttributeTypeException, "\n\
			metadata {\n\
				Width: { Min: \"aaa\" }\n\
			}\n\
		");
	}

	static void TestMinWidthNonInteger()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Width: { Min: 1.5 }\n\
			}\n\
		");
	}

	static void TestMinWidthTooSmall()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Width: { Min: 0 }\n\
			}\n\
		");
	}

	static void TestMinWidthTooLarge()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Width: { Min: 10000000000000 }\n\
			}\n\
		");
	}
	
	static void TestMaxWidth()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Width: { Max: 1000 }\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, 1000, params.GetMaxMapWidth());
	}

	static void TestMaxWidthNonNumber()
	{
		TEST_SCRIPT_FAILURE(IncorrectScriptParameterAttributeTypeException, "\n\
			metadata {\n\
				Width: { Max: \"aaa\" }\n\
			}\n\
		");
	}

	static void TestMaxWidthNonInteger()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Width: { Max: 1.5 }\n\
			}\n\
		");
	}

	static void TestMaxWidthTooSmall()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Width: { Max: 0 }\n\
			}\n\
		");
	}

	static void TestMaxWidthTooLarge()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Width: { Max: 10000000000000 }\n\
			}\n\
		");
	}

	static void TestDefaultWidthMinGreaterThanMax()
	{
		TEST_SCRIPT_FAILURE(MinGreaterThanMaxSizeException, "\n\
			metadata {\n\
				Width: { Min: 2000, Max: 1000 }\n\
			}\n\
		");
	}

	static void TestDefaultInfiniteHeight()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, MAP_SIZE_INFINITE, params.GetDefaultMapHeight());
		ASSERT_EQUALS(unsigned, MAP_SIZE_MIN, params.GetMinMapHeight());
		ASSERT_EQUALS(unsigned, MAP_SIZE_INFINITE, params.GetMaxMapHeight());
	}

	static void TestExplicitInfiniteHeight()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Height: Infinite\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, MAP_SIZE_INFINITE, params.GetDefaultMapHeight());
		ASSERT_EQUALS(unsigned, MAP_SIZE_MIN, params.GetMinMapHeight());
		ASSERT_EQUALS(unsigned, MAP_SIZE_INFINITE, params.GetMaxMapHeight());
	}

	static void TestDefaultHeight()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Height: { Default: 1000 }\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, 1000, params.GetDefaultMapHeight());
	}

	static void TestDefaultHeightNonNumber()
	{
		TEST_SCRIPT_FAILURE(IncorrectScriptParameterAttributeTypeException, "\n\
			metadata {\n\
				Height: { Default: \"aaa\" }\n\
			}\n\
		");
	}

	static void TestDefaultHeightNonInteger()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Height: { Default: 1.5 }\n\
			}\n\
		");
	}

	static void TestDefaultHeightTooSmall()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Height: { Default: 0 }\n\
			}\n\
		");
	}

	static void TestDefaultHeightTooLarge()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Height: { Default: 10000000000000 }\n\
			}\n\
		");
	}

	static void TestMinHeight()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Height: { Min: 1000 }\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, 1000, params.GetMinMapHeight());
	}

	static void TestMinHeightNonNumber()
	{
		TEST_SCRIPT_FAILURE(IncorrectScriptParameterAttributeTypeException, "\n\
			metadata {\n\
				Height: { Min: \"aaa\" }\n\
			}\n\
		");
	}

	static void TestMinHeightNonInteger()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Height: { Min: 1.5 }\n\
			}\n\
		");
	}

	static void TestMinHeightTooSmall()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Height: { Min: 0 }\n\
			}\n\
		");
	}

	static void TestMinHeightTooLarge()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Height: { Min: 10000000000000 }\n\
			}\n\
		");
	}
	
	static void TestMaxHeight()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			metadata {\n\
				Height: { Max: 1000 }\n\
			}\n\
		");
		
		ScriptParameters params = compiledScript->CreateScriptParameters();
		
		ASSERT_EQUALS(unsigned, 1000, params.GetMaxMapHeight());
	}

	static void TestMaxHeightNonNumber()
	{
		TEST_SCRIPT_FAILURE(IncorrectScriptParameterAttributeTypeException, "\n\
			metadata {\n\
				Height: { Max: \"aaa\" }\n\
			}\n\
		");
	}

	static void TestMaxHeightNonInteger()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Height: { Max: 1.5 }\n\
			}\n\
		");
	}

	static void TestMaxHeightTooSmall()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Height: { Max: 0 }\n\
			}\n\
		");
	}

	static void TestMaxHeightTooLarge()
	{
		TEST_SCRIPT_FAILURE(IncorrectMapSizeNumericValueException, "\n\
			metadata {\n\
				Height: { Max: 10000000000000 }\n\
			}\n\
		");
	}

	static void TestDefaultHeightMinGreaterThanMax()
	{
		TEST_SCRIPT_FAILURE(MinGreaterThanMaxSizeException, "\n\
			metadata {\n\
				Height: { Min: 2000, Max: 1000 }\n\
			}\n\
		");
	}

	MetadataTests() : TestFixtureBase("MetadataTests")
	{
		ADD_TESTCASE(TestGetStringMetadataValue);
		ADD_TESTCASE(TestGetIdentifierMetadataValue);
		ADD_TESTCASE(TestGetNumberMetadataValue);
		ADD_TESTCASE(TestGetBooleanMetadataValue);
		//ADD_TESTCASE(TestGetSimpleCollectionMetadataValue);
		ADD_TESTCASE(TestGetKeyValueCollectionMetadataValue);
		ADD_TESTCASE(TestMetadataKeyRedefinition);
		ADD_TESTCASE(TestParseBooleanScriptParameter);
		ADD_TESTCASE(TestParseBooleanScriptParameterWithImplicitAttributes);
		ADD_TESTCASE(TestParseNumberScriptParameter);
		ADD_TESTCASE(TestParseNumberScriptParameterWithImplicitAttributes);
		ADD_TESTCASE(TestUseBooleanScriptParameter);
		ADD_TESTCASE(TestUseBooleanScriptParameterWithDefaultValue);
		ADD_TESTCASE(TestUseNumberScriptParameter);
		ADD_TESTCASE(TestUseNumberScriptParameterWithDefaultValue);
		ADD_TESTCASE(TestDefaultInfiniteWidth);
		ADD_TESTCASE(TestExplicitInfiniteWidth);
		ADD_TESTCASE(TestDefaultWidth);
		ADD_TESTCASE(TestDefaultWidthNonNumber);
		ADD_TESTCASE(TestDefaultWidthNonInteger);
		ADD_TESTCASE(TestDefaultWidthTooSmall);
		ADD_TESTCASE(TestDefaultWidthTooLarge);
		ADD_TESTCASE(TestMinWidth);
		ADD_TESTCASE(TestMinWidthNonNumber);
		ADD_TESTCASE(TestMinWidthNonInteger);
		ADD_TESTCASE(TestMinWidthTooSmall);
		ADD_TESTCASE(TestMinWidthTooLarge);
		ADD_TESTCASE(TestMaxWidth);
		ADD_TESTCASE(TestMaxWidthNonNumber);
		ADD_TESTCASE(TestMaxWidthNonInteger);
		ADD_TESTCASE(TestMaxWidthTooSmall);
		ADD_TESTCASE(TestMaxWidthTooLarge);
		ADD_TESTCASE(TestDefaultInfiniteHeight);
		ADD_TESTCASE(TestExplicitInfiniteHeight);
		ADD_TESTCASE(TestDefaultHeight);
		ADD_TESTCASE(TestDefaultHeightNonNumber);
		ADD_TESTCASE(TestDefaultHeightNonInteger);
		ADD_TESTCASE(TestDefaultHeightTooSmall);
		ADD_TESTCASE(TestDefaultHeightTooLarge);
		ADD_TESTCASE(TestMinHeight);
		ADD_TESTCASE(TestMinHeightNonNumber);
		ADD_TESTCASE(TestMinHeightNonInteger);
		ADD_TESTCASE(TestMinHeightTooSmall);
		ADD_TESTCASE(TestMinHeightTooLarge);
		ADD_TESTCASE(TestMaxHeight);
		ADD_TESTCASE(TestMaxHeightNonNumber);
		ADD_TESTCASE(TestMaxHeightNonInteger);
		ADD_TESTCASE(TestMaxHeightTooSmall);
		ADD_TESTCASE(TestMaxHeightTooLarge);
	}
};