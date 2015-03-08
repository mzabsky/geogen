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