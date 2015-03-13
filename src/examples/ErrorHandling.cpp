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

void Example_ErrorHandling()
{
	/// [Body]
	string code = "yield HeightMap.Flat(1 / 0)";

	// Create instance of the compiler.
	compiler::Compiler compiler;

	// All compilation exceptions extend compiler::CompilerException.
	runtime::CompiledScript* compiledScript;
	try
	{
		compiledScript = compiler.CompileScript(code);
	}
	catch (compiler::CompilerException& ex)
	{
		cout << "Error GGE" << ex.GetErrorCode() << ": " << ex.GetDetailMessage() << endl;
		return;
	}

	// All simulation exceptions extend runtime::RuntimeException.
	runtime::VirtualMachine vm(*compiledScript);
	try
	{
		vm.Run();
	}
	catch (runtime::RuntimeException& ex)
	{
		cout << "Error GGE" << ex.GetErrorCode() << ": " << ex.GetDetailMessage() << endl;
		return;
	}

	renderer::Renderer renderer(vm.GetRenderingSequence());
	renderer.Run();

	genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(renderer::Renderer::MAP_NAME_MAIN);

	cout << "Finished!" << endl;
	/// [Body]
}