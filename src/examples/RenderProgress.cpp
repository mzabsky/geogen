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

#include <GeoGen/GeoGen.hpp>

using namespace std;
using namespace geogen;

void Example_RenderProgress()
{
	/// [Body]
	string code = "\n\
		var a = HeightMap.Flat(0.2); \n\
		var b = HeightMap.Flat(0.3); \n\
		a.Add(b); \n\
		yield a;";

	compiler::Compiler compiler;
	runtime::CompiledScript* compiledScript = compiler.CompileScript(code);

	runtime::VirtualMachine vm(*compiledScript);

	vm.Run();

	renderer::Renderer renderer(vm.GetRenderingSequence());

	// Instead of Run, the sequence is executed step by step.
	while (renderer.GetStatus() == renderer::RENDERER_STATUS_READY)
	{
		renderer.Step();

		cout << "Progress: " << renderer.GetProgress() << "%" << std::endl;
	}
	
	genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(renderer::Renderer::MAP_NAME_MAIN);

	cout << "Finished!" << std::endl;
	/// [Body]
}