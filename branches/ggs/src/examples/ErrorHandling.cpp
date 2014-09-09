#include <GeoGen/GeoGen.hpp>

using namespace std;

void Example_ErrorHandling()
{
	/// [Body]
	string code = "yield HeightMap.Flat(1 / 0)";

	// Create instance of the compiler.
	geogen::compiler::Compiler compiler;

	// All compilation exceptions extend geogen::compiler::CompilerException.
	geogen::runtime::CompiledScript* compiledScript;
	try
	{
		compiledScript = compiler.CompileScript(code);
	}
	catch (geogen::compiler::CompilerException& ex)
	{
		cout << "Error GGE" << ex.GetErrorCode() << ": " << ex.GetDetailMessage() << endl;
		return;
	}

	// All simulation exceptions extend geogen::runtime::RuntimeExceptions.
	geogen::runtime::VirtualMachine vm(*compiledScript);
	try
	{
		vm.Run();
	}
	catch (geogen::runtime::RuntimeException& ex)
	{
		cout << "Error GGE" << ex.GetErrorCode() << ": " << ex.GetDetailMessage() << endl;
		return;
	}

	geogen::renderer::Renderer renderer(vm.GetRenderingSequence());
	renderer.Run();

	geogen::genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(geogen::renderer::Renderer::MAP_NAME_MAIN);

	cout << "Finished!" << endl;
	/// [Body]
}