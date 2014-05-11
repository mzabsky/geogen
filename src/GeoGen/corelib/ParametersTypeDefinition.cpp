#include "ParametersTypeDefinition.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/ScriptParameter.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/BooleanScriptParameter.hpp"
#include "BooleanTypeDefinition.hpp"
#include "../runtime/NumberScriptParameter.hpp"
#include "NumberTypeDefinition.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;
using namespace corelib;

ParametersTypeDefinition::ParametersTypeDefinition(ScriptParameters const& parameters) : TypeDefinition("Parameters")
{
	for (ScriptParameters::const_iterator it = parameters.Begin(); it != parameters.End(); it++)
	{
		this->GetStaticVariableDefinitions().AddItem(new VariableDefinition(it->first, true));
	}
}

void ParametersTypeDefinition::Initialize(VirtualMachine* vm) const
{
	TypeDefinition::Initialize(vm);

	ManagedObject* staticObject = vm->GetStaticInstance(this->GetName());
	if (staticObject == NULL)
	{
		throw InternalErrorException("Paramters type not initialized properly (static instance missing).");
	}

	ScriptParameters formalParameters = vm->GetCompiledScript().CreateScriptParameters();

	for (ScriptParameters::iterator it = formalParameters.Begin(); it != formalParameters.End(); it++)
	{
		ScriptParameter const* actualArgument = vm->GetArguments().GetItem(it->first);

		switch (it->second->GetType())
		{	
		case SCRIPT_PARAMETER_TYPE_BOOLEAN:
			{
				BooleanScriptParameter const* typedParameter = dynamic_cast<BooleanScriptParameter const*>(actualArgument == NULL ? it->second : actualArgument);
				ManagedObject* object = vm->GetBooleanTypeDefinition()->CreateInstance(vm, typedParameter->GetValue());
				if (!staticObject->GetMemberVariableTable().DeclareVariable(it->first, object, true))
				{
					throw InternalErrorException("Parameter member name conflict.");
				}
				break;
			}
		case SCRIPT_PARAMETER_TYPE_NUMBER:
			{
				NumberScriptParameter const* typedParameter = dynamic_cast<NumberScriptParameter const*>(actualArgument == NULL ? it->second : actualArgument);
				ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, typedParameter->GetValue());
				if (!staticObject->GetMemberVariableTable().DeclareVariable(it->first, object, true))
				{
					throw InternalErrorException("Parameter member name conflict.");
				}
				break;
			}	
		default:
			throw InternalErrorException("Invalid script parameter type.");
		}
	}
}

ManagedObject* ParametersTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	return a;
}