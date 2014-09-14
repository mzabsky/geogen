#include <GeoGen/GeoGen.hpp>

using namespace std;
using namespace geogen;

void Example_ScriptParameters()
{
	/// [Body]
	string code = "\n\
		metadata \n\
		{ \n\
			ExampleParameter: {Type: Number, Restriction: Integers } \n\
		}; \n\
		yield HeightMap.Flat(); \n\
	";

	compiler::Compiler compiler;
	runtime::CompiledScript* compiledScript = compiler.CompileScript(code);

	// Create a ScriptParameters object from the script metadata.
	runtime::ScriptParameters parameters = compiledScript->CreateScriptParameters();

	// The parameters object can now be customized in any way (commonly it would be iterated over and rendered onto a GUI. The values from the GUI would then be set back into the object).
	// In this example, we will only adjust render size and set value of the single parameter.
	parameters.SetRenderHeight(2000);
	parameters.SetRenderHeight(1000);
	
	// Set the value of the parameter (we are assuming that the parameter exists and that it is of type Number).
	dynamic_cast<runtime::NumberScriptParameter*>(parameters.GetItem("ExampleParameter"))->SetValue(333);

	// Now pass the adjusted parameters to the VM.
	runtime::VirtualMachine vm(*compiledScript, parameters);

	vm.Run();

	renderer::Renderer renderer(vm.GetRenderingSequence());
	renderer.Run();
	genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(renderer::Renderer::MAP_NAME_MAIN);

	cout << "I have a height map with width " << heightMap->GetWidth() << " and height " << heightMap << " with data at 0x" << heightMap->GetHeightDataPtr() << "." << std::endl;
	/// [Body]
}