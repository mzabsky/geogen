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

/// [Include]
#include <GeoGen/GeoGen.hpp>
/// [Include]

using namespace std;
using namespace geogen;

void Example_BasicUsage()
{
/// [Body]
	string code = "yield HeightMap.Flat()";

	compiler::Compiler compiler;

	// Compile the script. Any compilation errors will cause an compiler::CompilerException to be thrown.
	runtime::CompiledScript* compiledScript = compiler.CompileScript(code); 

	// Create a virtual machine.
	runtime::VirtualMachine vm(*compiledScript);

	// Run the script. Any runtime errors will cause an runtime::RuntimeException to be thrown.
	vm.Run();

	// Create a new renderer for the rendering sequence that was created during execution of the VM.
	renderer::Renderer renderer(vm.GetRenderingSequence());

	// Render all the maps.
	renderer.Run();

	// Get the main height map.
	genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(renderer::Renderer::MAP_NAME_MAIN);

	cout << "I have a height map with width " << heightMap->GetWidth() << " and height " << heightMap << " with data at 0x" << heightMap->GetHeightDataPtr() << "." << std::endl;
/// [Body]
}