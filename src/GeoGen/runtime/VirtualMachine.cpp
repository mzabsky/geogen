#include "VirtualMachine.hpp"
#include "TypeDefinition.hpp"
#include "StaticObject.hpp"
#include "..\ApiUsageException.hpp"

using namespace geogen::runtime;

VirtualMachine::VirtualMachine(CompiledScript const& compiledScript)
: compiledScript(compiledScript), status(VIRTUAL_MACHINE_STATUS_READY)
{
	this->InitializeStaticObjects();
};

void VirtualMachine::InitializeStaticObjects()
{
	for (
		SymbolDefinitionTable<TypeDefinition>::const_iterator it = this->GetCompiledScript().GetTypeDefinitions().Begin(); 
		it != this->GetCompiledScript().GetTypeDefinitions().End(); 
		it++)
	{
		DynamicObject* staticObject = (*it).second->CreateStaticObject();
		this->GetMemoryManager().RegisterObject(staticObject);
	}
}

VirtualMachineStepResult VirtualMachine::Step()
{
	if (this->status == VIRTUAL_MACHINE_STATUS_READY)
	{
		throw ApiUsageException("The VM is in incorrect state.");
	}

	return VIRTUAL_MACHINE_STEP_RESULT_RUNNING;
}