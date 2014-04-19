#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\VariableRedefinitionException.hpp"

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
		ASSERT_EQUALS(MetadataType, METADATA_TYPE_BOOL, value->GetType());
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

		ASSERT_EQUALS(MetadataType, METADATA_TYPE_BOOL, (*(collection->Begin() + 2))->GetType());
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

		ASSERT_EQUALS(MetadataType, METADATA_TYPE_BOOL, collection->GetItem("BooleanKey")->GetType());
		ASSERT_EQUALS(bool, true, dynamic_cast<MetadataBoolean const*>(collection->GetItem("BooleanKey"))->GetValue());
	}

	MetadataTests() : TestFixtureBase("MetadataTests")
	{
		ADD_TESTCASE(TestGetStringMetadataValue);
		ADD_TESTCASE(TestGetNumberMetadataValue);
		ADD_TESTCASE(TestGetBooleanMetadataValue);
		//ADD_TESTCASE(TestGetSimpleCollectionMetadataValue);
		ADD_TESTCASE(TestGetKeyValueCollectionMetadataValue);
	}
};