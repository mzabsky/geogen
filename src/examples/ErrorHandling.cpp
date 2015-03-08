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