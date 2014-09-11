#include <GeoGen/GeoGen.hpp>

using namespace std;

/// [Callback]
void MyMessageHandler(geogen::runtime::VirtualMachine* virtualMachine, geogen::CodeLocation location, string const& formattedMessage, string const& unformattedMessage, vector<string> arguments)
{
	cout << "SCRIPT MESSAGE: " << formattedMessage << std::endl;
};
/// [Callback]

void Example_TextMessages()
{
	/// [Body]
	string code = "Print(\"Lorem ipsum {0}\", 5);yield HeightMap.Flat(1 / 0);";

	geogen::compiler::Compiler compiler;

	geogen::runtime::CompiledScript* compiledScript = compiledScript = compiler.CompileScript(code);	

	geogen::runtime::VirtualMachine vm(*compiledScript);
	
	// Before executing any code, assign the text message handler.
	vm.SetScriptMessageHandler(MyMessageHandler);

	vm.Run();

	geogen::renderer::Renderer renderer(vm.GetRenderingSequence());
	renderer.Run();

	geogen::genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(geogen::renderer::Renderer::MAP_NAME_MAIN);

	cout << "Finished!" << endl;
	/// [Body]
}