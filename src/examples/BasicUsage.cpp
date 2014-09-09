#include <GeoGen/GeoGen.hpp>

using namespace std;

void Example_BasicUsage()
{
/// [Body]
	string code = "yield HeightMap.Flat()";

	geogen::compiler::Compiler compiler;

	// Compile the script. Any compilation errors will cause an compiler::CompilerException to be thrown.
	geogen::runtime::CompiledScript* compiledScript = compiler.CompileScript(code); 

	// Create a virtual machine.
	geogen::runtime::VirtualMachine vm(*compiledScript);

	// Run the script. Any runtime errors will cause an runtime::RuntimeException to be thrown.
	vm.Run();

	// Create a new renderer for the rendering sequence that was created during execution of the VM.
	geogen::renderer::Renderer renderer(vm.GetRenderingSequence());

	// Render all the maps.
	renderer.Run();

	// Get the main height map.
	geogen::genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(geogen::renderer::Renderer::MAP_NAME_MAIN);

	cout << "I have a height map with width " << heightMap->GetWidth() << " and height " << heightMap << " with data at 0x" << heightMap->GetHeightDataPtr() << "." << std::endl;
/// [Body]
}