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