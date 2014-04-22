#include "VirtualMachine.hpp"
#include "TypeDefinition.hpp"
#include "VariableDefinition.hpp"
#include "StaticObject.hpp"
#include "BooleanTypeDefinition.hpp"
#include "../ApiUsageException.hpp"
#include "../InternalErrorException.hpp"
#include "NumberTypeDefinition.hpp"
#include "VirtualMachineStatusGuard.hpp"
#include "../CodeLocation.hpp"
#include "UndefinedSymbolAccessException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

VirtualMachine::VirtualMachine(CompiledScript const& compiledScript, ScriptParameters const& arguments)
: compiledScript(compiledScript), status(VIRTUAL_MACHINE_STATUS_READY), globalVariableTable(&memoryManager), arguments(&arguments)
{
	this->ValidateArguments();
	this->InitializeTypes();
	this->InitializeGlobalVariables();
	this->InitializeMainFunction();
};

void VirtualMachine::InitializeTypes()
{
	for (
		SymbolDefinitionTable<TypeDefinition>::const_iterator it = this->GetCompiledScript().GetTypeDefinitions().Begin(); 
		it != this->GetCompiledScript().GetTypeDefinitions().End(); 
		it++)
	{
		it->second->Initialize(this);
	}
}

void VirtualMachine::InitializeGlobalVariables()
{
	for (
		SymbolDefinitionTable<VariableDefinition>::const_iterator it = this->GetCompiledScript().GetGlobalVariableDefinitions().Begin();
		it != this->GetCompiledScript().GetGlobalVariableDefinitions().End();
		it++)
	{
		it->second->Initialize(this);
	}
}

void VirtualMachine::InitializeMainFunction()
{
	FunctionDefinition const* mainFunctionDefinition = this->GetCompiledScript().GetGlobalFunctionDefinitions().GetItem(CompiledScript::MAIN_FUNCTION_NAME);
	if (mainFunctionDefinition == NULL)
	{
		throw InternalErrorException("The script doesn't have a main function.");
	}

	this->CallFunction(CodeLocation(0, 0), mainFunctionDefinition, 0);
}

void VirtualMachine::ValidateArguments()
{
	ScriptParameters originalParameters;
	this->GetCompiledScript().CreateScriptParameters(originalParameters);

	for (ScriptParameters::const_iterator it = this->GetArguments().Begin(); it != this->GetArguments().End(); it++)
	{
		ScriptParameter* originalParameter = originalParameters.GetItem(it->first);
		if (originalParameter == NULL)
		{
			stringstream ss;
			ss << "Argument \"" << it->first << "\" was not declared by the script.";
			throw ApiUsageException(ss.str());
		}

		if (!it->second->EqualsTo(originalParameter))
		{
			stringstream ss;
			ss << "Argument \"" << it->first << "\" does not match the corresponding formal parameter declared by the script.";
			throw ApiUsageException(ss.str());
		}
	}
}

VirtualMachineStepResult VirtualMachine::Step()
{
	if (this->status != VIRTUAL_MACHINE_STATUS_READY)
	{
		throw ApiUsageException("The VM is in incorrect state.");
	}

	VirtualMachineStatusGuard statusGuard(this->status);

	CallStackEntry& callStackEntry = this->callStack.Top();

	CallStackEntryStepResult stepResult = callStackEntry.Step(this);
	
	if (stepResult == CALL_STACK_ENTRY_STEP_RESULT_FINISHED)
	{
		callStack.Pop();
	}

	VirtualMachineStepResult result;
	if (!callStack.IsEmpty())
	{		
		result = VIRTUAL_MACHINE_STEP_RESULT_RUNNING;
		statusGuard.SetGuardStatus(VIRTUAL_MACHINE_STATUS_READY);
	}
	else
	{		
		result = VIRTUAL_MACHINE_STEP_RESULT_FINISHED;
		statusGuard.SetGuardStatus(VIRTUAL_MACHINE_STATUS_FINISHED);
	}

	return result;
}

void VirtualMachine::CallFunction(CodeLocation location, FunctionDefinition const* functionDefinition, unsigned numberOfArguments)
{
	this->callStack.Push(location, functionDefinition);

	functionDefinition->Call(location, this, numberOfArguments);
}

void VirtualMachine::Run()
{
	while (this->status == VIRTUAL_MACHINE_STATUS_READY)
	{
		this->Step();
	}
}

DynamicObject* VirtualMachine::GetNull()
{
	VariableTableItem* variableTableItem = this->GetGlobalVariableTable().GetVariable("null");

	if (variableTableItem == NULL)
	{
		throw InternalErrorException("Could not get \"null\" value.");
	}

	return variableTableItem->GetValue();
}

BooleanTypeDefinition const* VirtualMachine::GetBooleanTypeDefinition() const
{
	BooleanTypeDefinition const* booleanTypeDefinition = dynamic_cast<BooleanTypeDefinition const*>(this->GetCompiledScript().GetTypeDefinitions().GetItem("Boolean"));
	if (booleanTypeDefinition == NULL)
	{
		throw InternalErrorException("Could not get \"Boolean\" type definition.");
	}

	return booleanTypeDefinition;
}

NumberTypeDefinition const* VirtualMachine::GetNumberTypeDefinition() const
{
	NumberTypeDefinition const* numberTypeDefinition = dynamic_cast<NumberTypeDefinition const*>(this->GetCompiledScript().GetTypeDefinitions().GetItem("Number"));
	if (numberTypeDefinition == NULL)
	{
		throw InternalErrorException("Could not get \"Boolean\" type definition.");
	}

	return numberTypeDefinition;
}

VariableTableItem* VirtualMachine::FindVariable(std::string const& variableName)
{
	if (this->status != VIRTUAL_MACHINE_STATUS_READY)
	{
		throw ApiUsageException("The VM is in incorrect state.");
	}

	CodeBlockStack& codeBlockStack = this->GetCallStack().Top().GetCodeBlockStack();

	VariableTableItem* foundVariable = NULL;
	CodeBlockStack::reverse_iterator it = codeBlockStack.RBegin();
	while (foundVariable == NULL && it != codeBlockStack.REnd())
	{
		if ((*it)->GetLocalVariableTable().IsVariableDeclared(variableName))
		{
			foundVariable = (*it)->GetLocalVariableTable().GetVariable(variableName);
		}

		it++;
	}

	// Not found among local variables, look into global variables.
	if (foundVariable == NULL)
	{
		if (this->GetGlobalVariableTable().IsVariableDeclared(variableName))
		{
			foundVariable = this->GetGlobalVariableTable().GetVariable(variableName);
		}
	}

	return foundVariable;
}

DynamicObject* VirtualMachine::GetStaticInstance(std::string const& typeName)
{
	VariableTableItem* variableTableItem = this->GetGlobalVariableTable().GetVariable(typeName);

	if (variableTableItem == NULL)
	{
		return NULL;
	}

	else return variableTableItem->GetValue();
}