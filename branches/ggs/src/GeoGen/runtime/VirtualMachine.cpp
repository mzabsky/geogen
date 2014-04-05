#include "VirtualMachine.hpp"
#include "TypeDefinition.hpp"
#include "VariableDefinition.hpp"
#include "StaticObject.hpp"
#include "BooleanTypeDefinition.hpp"
#include "..\ApiUsageException.hpp"
#include "..\InternalErrorException.hpp"
#include "NumberTypeDefinition.hpp"

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

	this->CallFunction(mainFunctionDefinition);
}

VirtualMachineStepResult VirtualMachine::Step()
{
	if (this->status != VIRTUAL_MACHINE_STATUS_READY)
	{
		throw ApiUsageException("The VM is in incorrect state.");
	}

	CallStackEntry& callStackEntry = this->callStack.top();

	std::cout << "VM STEP - " << callStackEntry.GetFunctionDefinition()->GetName() << " " << &callStackEntry << "" <<  std::endl;

	CallStackEntryStepResult stepResult = callStackEntry.Step(this);
	
	std::cout << "VM STEP RESULT - " << stepResult << std::endl;

	if (stepResult == CALL_STACK_ENTRY_STEP_RESULT_FINISHED)
	{
		callStack.pop();
	}

	if (callStack.size() > 0)
	{
		return VIRTUAL_MACHINE_STEP_RESULT_RUNNING;
	}
	else
	{
		this->status = VIRTUAL_MACHINE_STATUS_FINISHED;
		return VIRTUAL_MACHINE_STEP_RESULT_FINISHED;
	}
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