#include <GeoGen/GeoGen.hpp>

using namespace std;

void Example_ScriptParameters()
{
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
	/// [Body]
	geogen::runtime::ScriptParameters parameters = compiledScript->CreateScriptParameters();

	// Iterate over all available parameters.
	for (geogen::runtime::ScriptParameters::iterator it = parameters.Begin(); it != parameters.End(); it++)
	{
		// For each item, a pair of parameter name and parameter object is returned.

		// Each type needs different processing.
		switch (it->second->GetType())
		{
		case geogen::runtime::SCRIPT_PARAMETER_TYPE_BOOLEAN:
			geogen::runtime::BooleanScriptParameter* booleanParameter = dynamic_cast<geogen::runtime::BooleanScriptParameter*>(it->second);
			
			cout << it->first << " (0/1): ";

			bool booleanValue;
			cin >> booleanValue;

			cout << endl;

			booleanParameter->SetValue(booleanValue);
			break;
		case geogen::runtime::SCRIPT_PARAMETER_TYPE_NUMBER:
			geogen::runtime::NumberScriptParameter* numberParameter = dynamic_cast<geogen::runtime::NumberScriptParameter*>(it->second);

			cout << it->first << " (" << numberParameter->GetMin() << " - " << numberParameter->GetMax() << "): ";

			double numberValue;
			cin >> numberValue;

			cout << endl;

			numberParameter->SetValue(numberValue);
			break;
		case geogen::runtime::SCRIPT_PARAMETER_TYPE_ENUM:
			geogen::runtime::EnumScriptParameter* enumParameter = dynamic_cast<geogen::runtime::EnumScriptParameter*>(it->second);

			cout << it->first << " (" << endl; 

			// Print list of possible values. The list of possible string values an enum can hold can be obtained from its enum type.
			for (
				geogen::corelib::EnumTypeDefinition::ValueDefinitions::const_iterator it2 = enumParameter->GetEnumType()->GetValueDefinitions().begin();
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

	geogen::runtime::VirtualMachine vm(*compiledScript, parameters);
	/// [Body]

	// Now pass the adjusted parameters to the VM.
	geogen::runtime::VirtualMachine vm(*compiledScript, parameters);

	vm.Run();

	geogen::renderer::Renderer renderer(vm.GetRenderingSequence());
	renderer.Run();
	geogen::genlib::HeightMap* heightMap = renderer.GetRenderedMapTable().GetItem(geogen::renderer::Renderer::MAP_NAME_MAIN);

	cout << "I have a height map with width " << heightMap->GetWidth() << " and height " << heightMap << " with data at 0x" << heightMap->GetHeightDataPtr() << "." << std::endl;
}