#include "../GeoGen/GeoGen.hpp"

using namespace std;

void Example_RenderProgress()
{
	/// [Body]
	string code = "\n\
		var a = HeightMap.Flat(0.2); \n\
		var b = HeightMap.Flat(0.3); \n\
		a.Add(b); \n\
		yield a;";

	geogen::compiler::Compiler compiler;
	geogen::runtime::CompiledScript* compiledScript = compiler.CompileScript(code);

	geogen::runtime::VirtualMachine vm(*compiledScript);

	vm.Run();

	geogen::renderer::Renderer renderer(vm.GetRenderingSequence());

	// Instead of Run, the sequence is executed step by step.
	while (renderer.GetStatus() == geogen::renderer::RENDERER_STATUS_READY)
	{
		renderer.Step();

		cout << "Progress: " << renderer.GetProgress() << "%" << std::endl;
	}
	
	geogen::genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(geogen::renderer::Renderer::MAP_NAME_MAIN);

	cout << "Finished!" << std::endl;
	/// [Body]
}