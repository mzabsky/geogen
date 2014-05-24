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

	if (vm->GetArguments().GetMapWidth() != MAP_SIZE_INFINITE){
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetMapWidth());
		if (!staticObject->GetMemberVariableTable().DeclareVariable("MapWidth", object, true))
		{
			throw InternalErrorException("MapWidth parameter member name conflict.");
		}
	}

	if (vm->GetArguments().GetMapHeight() != MAP_SIZE_INFINITE){
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetMapHeight());
		if (!staticObject->GetMemberVariableTable().DeclareVariable("MapHeight", object, true))
		{
			throw InternalErrorException("MapHeight parameter member name conflict.");
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderOriginX());
		if (!staticObject->GetMemberVariableTable().DeclareVariable("RenderOriginX", object, true))
		{
			throw InternalErrorException("RenderOriginX parameter member name conflict.");
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderOriginY());
		if (!staticObject->GetMemberVariableTable().DeclareVariable("RenderOriginY", object, true))
		{
			throw InternalErrorException("RenderOriginY parameter member name conflict.");
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderWidth());
		if (!staticObject->GetMemberVariableTable().DeclareVariable("RenderWidth", object, true))
		{
			throw InternalErrorException("RenderWidth parameter member name conflict.");
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderHeight());
		if (!staticObject->GetMemberVariableTable().DeclareVariable("RenderHeight", object, true))
		{
			throw InternalErrorException("RenderHeight parameter member name conflict.");
		}
	}

	{
		ManagedObject* object = vm->GetNumberTypeDefinition()->CreateInstance(vm, vm->GetArguments().GetRenderScale());
		if (!staticObject->GetMemberVariableTable().DeclareVariable("RenderScale", object, true))
		{
			throw InternalErrorException("RenderScale parameter member name conflict.");
		}
	}
}

ManagedObject* ParametersTypeDefinition::Copy(VirtualMachine* vm, ManagedObject* a) const
{
	return a;
}