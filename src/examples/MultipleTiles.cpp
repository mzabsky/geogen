#include <GeoGen/GeoGen.hpp>

using namespace std;

void Example_MultipleTiles()
{
	/// [Body]
	string code = "\n\
		metadata \n\
		{ \n\
			Width: Finite, \n\
			Height: Finite \n\
		}\n\
		yield HeightMap.RadialGradient(@[0.5, 0.5], Parameters.MapWidth * 0.4, 1, 0);";

	geogen::compiler::Compiler compiler;
	geogen::runtime::CompiledScript* compiledScript = compiler.CompileScript(code);

	// Prepare the render rectangles.
	vector<geogen::Rectangle> rectangles;
	rectangles.push_back(geogen::Rectangle(geogen::Point(0, 0), geogen::Size2D(1000, 1000)));
	rectangles.push_back(geogen::Rectangle(geogen::Point(1000, 0), geogen::Size2D(1000, 1000)));
	rectangles.push_back(geogen::Rectangle(geogen::Point(0, 1000), geogen::Size2D(1000, 1000)));
	rectangles.push_back(geogen::Rectangle(geogen::Point(1000, 1000), geogen::Size2D(1000, 1000)));

	vector<geogen::genlib::HeightMap*> renderedMaps;

	// Render each rectangle individually.
	for (unsigned i = 0; i < rectangles.size(); i++)
	{
		// The render region is specified using script parameters.
		geogen::runtime::ScriptParameters parameters = compiledScript->CreateScriptParameters();

		// Set extents of the map area (because the map is finite, this is not necessary/possible for infinite maps).
		parameters.SetMapWidth(2000);
		parameters.SetMapHeight(2000);

		// Set the render rectangle.
		parameters.SetRenderRectangle(rectangles[i]);

		// Now run the script and render as usual.
		geogen::runtime::VirtualMachine vm(*compiledScript, parameters);
		vm.Run();

		geogen::renderer::Renderer renderer(vm.GetRenderingSequence());
		renderer.Run();

		// Ownership of the rendered maps has to be transferred from the renderer to us (otherwise the map would be deleted when the renderer is released).
		// The RemoveItem call will remove the pointers from the rendered map table, so the maps will no longer be associated with the renderer.
		renderedMaps.push_back(renderer.GetRenderedMapTable().GetItem(geogen::renderer::Renderer::MAP_NAME_MAIN));

		renderer.GetRenderedMapTable().RemoveItem(geogen::renderer::Renderer::MAP_NAME_MAIN);
	}

	cout << "Rendered " << renderedMaps.size() << " beautiful maps." << std::endl << std::endl;

	// Since the renderer no longer has ownership of the maps, make sure to release them when no longer needed.
	for (unsigned i = 0; i < renderedMaps.size(); i++)
	{
		delete renderedMaps[i];
	}

	/// [Body]
}