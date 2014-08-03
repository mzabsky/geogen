#pragma once

#include "TestFixtureBase.hpp"

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

	static void TestAddTwoMaps()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			var heightMap = HeightMap.Flat(0.5); \n\
			var addend = HeightMap.Flat(0.1); \n\
			heightMap.Add(addend); \n\
			yield heightMap; \n\
		");

		ScriptParameters parameters = compiledScript->CreateScriptParameters();
		parameters.SetRenderWidth(5);
		parameters.SetRenderHeight(5);

		VirtualMachine vm(*compiledScript, parameters);
		vm.Run();

		RenderingSequence& renderingSequence = vm.GetRenderingSequence();

		Renderer renderer(renderingSequence);
		renderer.CalculateRenderingBounds();
		renderer.Run();

		ASSERT_EQUALS(bool, true, renderer.GetRenderedMapTable().ContainsItem(Renderer::MAP_NAME_MAIN));
		ASSERT_EQUALS(Height, 19659, (*renderer.GetRenderedMapTable().GetItem(Renderer::MAP_NAME_MAIN))(4, 4));

		SaveRenders("TestAddTwoMaps", renderer.GetRenderedMapTable());
	}

	static void TestSimpleTiling()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			var heightMap = HeightMap.RadialGradient([400, 400], 350, 1.0, 0.0); \n\
			yield heightMap; \n\
		");

		vector<pair<Coordinate, Coordinate>> positions;
		positions.push_back(pair<Coordinate, Coordinate>(0, 0));
		positions.push_back(pair<Coordinate, Coordinate>(0, 250));
		positions.push_back(pair<Coordinate, Coordinate>(0, 500));
		positions.push_back(pair<Coordinate, Coordinate>(250, 0));
		positions.push_back(pair<Coordinate, Coordinate>(250, 250));
		positions.push_back(pair<Coordinate, Coordinate>(250, 500));
		positions.push_back(pair<Coordinate, Coordinate>(500, 0));
		positions.push_back(pair<Coordinate, Coordinate>(500, 250));
		positions.push_back(pair<Coordinate, Coordinate>(500, 500));

		for (vector<pair<Coordinate, Coordinate>>::iterator it = positions.begin(); it != positions.end(); it++)
		{
			ScriptParameters parameters = compiledScript->CreateScriptParameters();
			parameters.SetRenderWidth(250);
			parameters.SetRenderHeight(250);
			parameters.SetRenderOriginX(it->first);
			parameters.SetRenderOriginY(it->second);

			VirtualMachine vm(*compiledScript, parameters);
			vm.Run();

			RenderingSequence& renderingSequence = vm.GetRenderingSequence();

			Renderer renderer(renderingSequence);
			renderer.CalculateRenderingBounds();
			renderer.Run();

			ASSERT_EQUALS(bool, true, renderer.GetRenderedMapTable().ContainsItem(Renderer::MAP_NAME_MAIN));
			//ASSERT_EQUALS(Height, 19659, (*renderer.GetRenderedMapTable().GetItem(Renderer::MAP_NAME_MAIN))(4, 4));

			SaveRenders("TestSimpleTiling", renderer.GetRenderedMapTable());
		}
	}

	static void TestTilingWithScaling()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			var heightMap = HeightMap.RadialGradient([400, 400], 950, 1.0, 0.0); \n\
			yield heightMap; \n\
		");

		vector<pair<Coordinate, Coordinate>> positions;
		positions.push_back(pair<Coordinate, Coordinate>(0, 0));
		positions.push_back(pair<Coordinate, Coordinate>(0, 250));
		positions.push_back(pair<Coordinate, Coordinate>(0, 500));
		positions.push_back(pair<Coordinate, Coordinate>(250, 0));
		positions.push_back(pair<Coordinate, Coordinate>(250, 250));
		positions.push_back(pair<Coordinate, Coordinate>(250, 500));
		positions.push_back(pair<Coordinate, Coordinate>(500, 0));
		positions.push_back(pair<Coordinate, Coordinate>(500, 250));
		positions.push_back(pair<Coordinate, Coordinate>(500, 500));

		for (vector<pair<Coordinate, Coordinate>>::iterator it = positions.begin(); it != positions.end(); it++)
		{
			ScriptParameters parameters = compiledScript->CreateScriptParameters();
			parameters.SetRenderWidth(250);
			parameters.SetRenderHeight(250);
			parameters.SetRenderOriginX(it->first);
			parameters.SetRenderOriginY(it->second);
			parameters.SetRenderScale(0.5);

			VirtualMachine vm(*compiledScript, parameters);
			vm.Run();

			RenderingSequence& renderingSequence = vm.GetRenderingSequence();

			Renderer renderer(renderingSequence);
			renderer.CalculateRenderingBounds();
			renderer.Run();

			ASSERT_EQUALS(bool, true, renderer.GetRenderedMapTable().ContainsItem(Renderer::MAP_NAME_MAIN));
			//ASSERT_EQUALS(Height, 19659, (*renderer.GetRenderedMapTable().GetItem(Renderer::MAP_NAME_MAIN))(4, 4));

			SaveRenders("TestTilingWithScaling", renderer.GetRenderedMapTable());
		}
	}


	static void TestBlur()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			var heightMap = HeightMap.RadialGradient([250, 250], 100, 1.0, 0.0); \n\
			heightMap.Blur(20); \n\
			yield heightMap; \n\
		");

		ScriptParameters parameters = compiledScript->CreateScriptParameters();
		parameters.SetRenderWidth(500);
		parameters.SetRenderHeight(500);

		VirtualMachine vm(*compiledScript, parameters);
		vm.Run();

		RenderingSequence& renderingSequence = vm.GetRenderingSequence();

		Renderer renderer(renderingSequence);
		renderer.CalculateRenderingBounds();
		renderer.Run();

		ASSERT_EQUALS(bool, true, renderer.GetRenderedMapTable().ContainsItem(Renderer::MAP_NAME_MAIN));

		SaveRenders("TestBlur", renderer.GetRenderedMapTable());
	}

	static void TestNoise()
	{
		auto_ptr<CompiledScript> compiledScript = TestGetCompiledScript("\n\
			var heightMap = HeightMap.Noise(); \n\
			yield heightMap; \n\
		");

		ScriptParameters parameters = compiledScript->CreateScriptParameters();
		parameters.SetRenderWidth(500);
		parameters.SetRenderHeight(500);

		VirtualMachine vm(*compiledScript, parameters);
		vm.Run();

		RenderingSequence& renderingSequence = vm.GetRenderingSequence();

		Renderer renderer(renderingSequence);
		renderer.CalculateRenderingBounds();
		renderer.Run();

		ASSERT_EQUALS(bool, true, renderer.GetRenderedMapTable().ContainsItem(Renderer::MAP_NAME_MAIN));

		SaveRenders("TestNoise", renderer.GetRenderedMapTable());
	}

	RendererTests() : TestFixtureBase("RendererTests")
	{
		ADD_TESTCASE(TestSimpleRender);
		ADD_TESTCASE(TestAddTwoMaps);
		ADD_TESTCASE(TestSimpleTiling);
		ADD_TESTCASE(TestTilingWithScaling);
		ADD_TESTCASE(TestBlur);
		ADD_TESTCASE(TestNoise);
	}
};