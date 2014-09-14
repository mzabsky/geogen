#include <GeoGen/GeoGen.hpp>

using namespace std;
using namespace geogen;

void Example_ScriptParameters()
{
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
	/// [Body]
	runtime::ScriptParameters parameters = compiledScript->CreateScriptParameters();

	// Iterate over all available parameters.
	for (runtime::ScriptParameters::iterator it = parameters.Begin(); it != parameters.End(); it++)
	{
		// For each item, a pair of parameter name and parameter object is returned.

		// Each type needs different processing.
		switch (it->second->GetType())
		{
		case runtime::SCRIPT_PARAMETER_TYPE_BOOLEAN:
			runtime::BooleanScriptParameter* booleanParameter = dynamic_cast<runtime::BooleanScriptParameter*>(it->second);
			
			cout << it->first << " (0/1): ";

			bool booleanValue;
			cin >> booleanValue;

			cout << endl;

			booleanParameter->SetValue(booleanValue);
			break;
		case runtime::SCRIPT_PARAMETER_TYPE_NUMBER:
			runtime::NumberScriptParameter* numberParameter = dynamic_cast<runtime::NumberScriptParameter*>(it->second);

			cout << it->first << " (" << numberParameter->GetMin() << " - " << numberParameter->GetMax() << "): ";

			double numberValue;
			cin >> numberValue;

			cout << endl;

			numberParameter->SetValue(numberValue);
			break;
		case runtime::SCRIPT_PARAMETER_TYPE_ENUM:
			runtime::EnumScriptParameter* enumParameter = dynamic_cast<runtime::EnumScriptParameter*>(it->second);

			cout << it->first << " (" << endl; 

			// Print list of possible values. The list of possible string values an enum can hold can be obtained from its enum type.
			for (
				corelib::EnumTypeDefinition::ValueDefinitions::const_iterator it2 = enumParameter->GetEnumType()->GetValueDefinitions().begin();
				it2 != enumParameter->GetEnumType()->GetValueDefinitions().end();
				it2++
			)
			{
				// it2 is a pair of a string value and the corresponding integer value.

				cout << it->first << ", ";
			}

			cout << "): ";

			string stringValue;
			cin >> stringValue;
			enumParameter->SetValue(stringValue);
			
			cout << endl;

			break;
		}
	}

	runtime::VirtualMachine vm(*compiledScript, parameters);
	/// [Body]

	// Now pass the adjusted parameters to the VM.
	runtime::VirtualMachine vm(*compiledScript, parameters);

	vm.Run();

	renderer::Renderer renderer(vm.GetRenderingSequence());
	renderer.Run();
	genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(renderer::Renderer::MAP_NAME_MAIN);

	cout << "I have a height map with width " << heightMap->GetWidth() << " and height " << heightMap << " with data at 0x" << heightMap->GetHeightDataPtr() << "." << std::endl;
}