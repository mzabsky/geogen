#include "VirtualMachine.hpp"
#include "TypeDefinition.hpp"
#include "VariableDefinition.hpp"
#include "StaticObject.hpp"
#include "..\ApiUsageException.hpp"
#include "..\InternalErrorException.hpp"

using namespace geogen::runtime;

VirtualMachine::VirtualMachine(CompiledScript const& compiledScript)
: compiledScript(compiledScript), status(VIRTUAL_MACHINE_STATUS_READY)
{
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
		it->second->Initialize(*this);
	}
}

void VirtualMachine::InitializeGlobalVariables()
{
	for (
		SymbolDefinitionTable<VariableDefinition>::const_iterator it = this->GetCompiledScript().GetGlobalVariableDefinitions().Begin();
		it != this->GetCompiledScript().GetGlobalVariableDefinitions().End();
		it++)
	{
		it->second->Initialize(*this);
	}
}

void VirtualMachine::InitializeMainFunction()
{
	FunctionDefinition const* mainFunctionDefinition = this->GetCompiledScript().GetGlobalFunctionDefinitions().GetItem(CompiledScript::MAIN_FUNCTION_NAME);
	if (mainFunctionDefinition == NULL)
	{
		throw InternalErrorException("The script doesn't have a main function.");
	}

	this->CallFunction(mainFunctionDefinition);
}

VirtualMachineStepResult VirtualMachine::Step()
{
	if (this->status != VIRTUAL_MACHINE_STATUS_READY)
	{
		throw ApiUsageException("The VM is in incorrect state.");
	}

	CallStackEntry callStackEntry = this->callStack.top();
	CallStackEntryStepResult stepResult = callStackEntry.Step(this);

	return VIRTUAL_MACHINE_STEP_RESULT_RUNNING;
}

void VirtualMachine::CallFunction(FunctionDefinition const* functionDefinition)
{
	CallStackEntry callStackEntry(functionDefinition);
	this->callStack.push(callStackEntry);

	functionDefinition->Call(this);
}

void VirtualMachine::Run()
{
	while (this->status == VIRTUAL_MACHINE_STATUS_READY)
	{
		this->Step();
	}
}