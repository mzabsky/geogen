#include "../GeoGen/GeoGen.hpp"

using namespace std;

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

	geogen::compiler::Compiler compiler;
	geogen::runtime::CompiledScript* compiledScript = compiler.CompileScript(code);

	// Create a ScriptParameters object from the script metadata.
	geogen::runtime::ScriptParameters parameters = compiledScript->CreateScriptParameters();

	// The parameters object can now be customized in any way (commonly it would be iterated over and rendered onto a GUI. The values from the GUI would then be set back into the object).
	// In this example, we will only adjust render size and set value of the single parameter.
	parameters.SetRenderHeight(2000);
	parameters.SetRenderHeight(1000);
	
	// Each parameter can be of one of three types: geogen::runtime::BoolanScriptParameter, geogen::runtime::NumberScriptParameter and geogen::runtime::EnumScriptParameter.
	// The actual type can be determined by calling GetType on the base class.
	dynamic_cast<geogen::runtime::NumberScriptParameter*>(parameters.GetItem("ExampleParameter"))->SetValue(333);

	// Now pass the adjusted parameters to the VM.
	geogen::runtime::VirtualMachine vm(*compiledScript, parameters);

	vm.Run();

	geogen::renderer::Renderer renderer(vm.GetRenderingSequence());
	renderer.Run();
	geogen::genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(geogen::renderer::Renderer::MAP_NAME_MAIN);

	cout << "I have a height map with width " << heightMap->GetWidth() << " and height " << heightMap << " with data at 0x" << heightMap->GetHeightDataPtr() << "." << std::endl;
	/// [Body]
}