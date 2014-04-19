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

	MetadataTests() : TestFixtureBase("MetadataTests")
	{
		ADD_TESTCASE(TestGetStringMetadataValue);
		ADD_TESTCASE(TestGetNumberMetadataValue);
		ADD_TESTCASE(TestGetBooleanMetadataValue);
	}
};