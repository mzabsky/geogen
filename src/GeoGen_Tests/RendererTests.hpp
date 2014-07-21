#pragma once

#include "TestFixtureBase.hpp"
#include "..\GeoGen\runtime\VariableRedefinitionException.hpp"
#include "..\GeoGen\runtime\BooleanScriptParameter.hpp"
#include "..\GeoGen\runtime\NumberScriptParameter.hpp"
#include "..\GeoGen\runtime\EnumScriptParameter.hpp"
#include "..\GeoGen\compiler\IncorrectScriptParameterAttributeTypeException.hpp"
#include "..\GeoGen\compiler\IncorrectMapSizeNumericValueException.hpp"
#include "..\GeoGen\compiler\MinGreaterThanMaxSizeException.hpp"

class RendererTests : public TestFixtureBase
{
private:
public:
	static void TestSimpleRender()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			var heightMap = HeightMap.Flat(0.5); \n\
			yield heightMap; \n\
		");

		ScriptParameters parameters = compiledScript->CreateScriptParameters();
		parameters.SetRenderWidth(500);
		parameters.SetRenderHeight(500);

		VirtualMachine vm(*compiledScript, parameters);
		vm.Run();

		RenderingSequence& renderingSequence = vm.GetRenderingSequence();

		Renderer renderer(renderingSequence);
		renderer.Run();

		ASSERT_EQUALS(bool, true, renderer.GetRenderedMapTable().ContainsItem(Renderer::MAP_NAME_MAIN));
	}

	RendererTests() : TestFixtureBase("RendererTests")
	{
		ADD_TESTCASE(TestSimpleRender);
	}
};