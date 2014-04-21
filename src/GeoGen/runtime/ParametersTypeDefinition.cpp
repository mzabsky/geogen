#include "ParametersTypeDefinition.hpp"
#include "VirtualMachine.hpp"
#include "DynamicObject.hpp"
#include "ScriptParameter.hpp"
#include "..\InternalErrorException.hpp"
#include "BooleanScriptParameter.hpp"
#include "BooleanTypeDefinition.hpp"
#include "NumberScriptParameter.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

ParametersTypeDefinition::ParametersTypeDefinition(CodeLocation location, ScriptParameters const& parameters) : TypeDefinition("Parameters")
{
	for (ScriptParameters::const_iterator it = parameters.Begin(); it != parameters.End(); it++)
	{
		this->GetStaticVariableDefinitions().AddItem(new VariableDefinition(it->first, true));
	}
}

void ParametersTypeDefinition::Initialize(VirtualMachine* vm) const
{
	TypeDefinition::Initialize(vm);

	DynamicObject* staticObject = vm->GetStaticInstance(this->GetName());
	if (staticObject)
	{
		throw InternalErrorException("Paramters type not initialized properly (static instance missing).");
	}

	for (ScriptParameters::const_iterator it = vm->GetArguments().Begin(); it != vm->GetArguments().End(); it++)
	{
		switch (it->second->GetType())
		{	
		case SCRIPT_PARAMETER_TYPE_BOOLEAN:
			{
				BooleanScriptParameter const* typedParameter = dynamic_cast<BooleanScriptParameter const*>(it->second);
				DynamicObject* object = vm->GetBooleanTypeDefinition()->CreateInstance(typedParameter->GetValue());
				staticObject->SetMemberValue(*vm, CodeLocation(0, 0), it->first, object);
				break;
			}
		case SCRIPT_PARAMETER_TYPE_NUMBER:
			{
				NumberScriptParameter const* typedParameter = dynamic_cast<NumberScriptParameter const*>(it->second);
				DynamicObject* object = vm->GetNumberTypeDefinition()->CreateInstance(typedParameter->GetValue());
				staticObject->SetMemberValue(*vm, CodeLocation(0, 0), it->first, object);
				break;
			}	
		default:
			throw InternalErrorException("Invalid script parameter type.");
		}
	}
}

DynamicObject* ParametersTypeDefinition::Copy(DynamicObject* a) const
{
	return a;
}