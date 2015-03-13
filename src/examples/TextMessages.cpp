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

/// [Callback]
void MyMessageHandler(runtime::VirtualMachine* virtualMachine, CodeLocation location, string const& formattedMessage, string const& unformattedMessage, vector<string> arguments)
{
	cout << "SCRIPT MESSAGE: " << formattedMessage << std::endl;
};
/// [Callback]

void Example_TextMessages()
{
	/// [Body]
	string code = "Print(\"Lorem ipsum {0}\", 5);yield HeightMap.Flat(1 / 0);";

	compiler::Compiler compiler;

	runtime::CompiledScript* compiledScript = compiledScript = compiler.CompileScript(code);	

	runtime::VirtualMachine vm(*compiledScript);
	
	// Before executing any code, assign the text message handler.
	vm.SetScriptMessageHandler(MyMessageHandler);

	vm.Run();

	renderer::Renderer renderer(vm.GetRenderingSequence());
	renderer.Run();

	genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(renderer::Renderer::MAP_NAME_MAIN);

	cout << "Finished!" << endl;
	/// [Body]
}