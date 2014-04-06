#pragma once

#include <vector>
#include <string>
#include <stack>

#include "CompiledScript.hpp"
#include "CallStackEntry.hpp"
#include "MemoryManager.hpp"
#include "VariableTable.hpp"
#include "CallStack.hpp"
#include "ObjectStack.hpp"

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;
		class BooleanTypeDefinition;
		class NumberTypeDefinition;
		class VariableTableItem;

		enum VirtualMachineStatus
		{
			VIRTUAL_MACHINE_STATUS_READY,
			VIRTUAL_MACHINE_STATUS_FINISHED,
			VIRTUAL_MACHINE_STATUS_FAULTED
		};

		enum VirtualMachineStepResult
		{
			VIRTUAL_MACHINE_STEP_RESULT_RUNNING,
			VIRTUAL_MACHINE_STEP_RESULT_FINISHED
		};

		class VirtualMachine
		{
		private:
			VirtualMachineStatus status;
			
			ObjectStack objectStack;
			CallStack callStack;

			CompiledScript const& compiledScript;

			MemoryManager memoryManager;
			VariableTable globalVariableTable;

			void InitializeTypes();
			void InitializeGlobalVariables();
			void InitializeMainFunction();

			// Non-copyable
			VirtualMachine(VirtualMachine const&) : globalVariableTable(NULL), compiledScript(compiledScript) {};
			VirtualMachine& operator=(VirtualMachine const&) {};
		public:
			VirtualMachine(CompiledScript const& compiledScript);
			~VirtualMachine() {};

			VirtualMachineStatus GetStatus() const { return this->status; }
			MemoryManager& GetMemoryManager() { return this->memoryManager; }
			VariableTable& GetGlobalVariableTable() { return this->globalVariableTable; }

			inline CompiledScript const& GetCompiledScript() const { return this->compiledScript; };

			inline ObjectStack& GetObjectStack() { return this->objectStack; };
			//inline std::stack<DynamicObject const*> const& GetObjectStack() const { return *((std::stack<DynamicObject const*>*)&this->objectStack); };

			inline CallStack& GetCallStack() { return this->callStack; };
			//inline std::stack<CallStackEntry const> const& GetCallStack() const { return *((std::stack<DynamicObject const*>*)&this->callStack); };

			VirtualMachineStepResult Step();

			void CallFunction(FunctionDefinition const* functionDefintion);

			void Run();

			DynamicObject* GetNull();
			BooleanTypeDefinition const* GetBooleanTypeDefinition() const;
			NumberTypeDefinition const* GetNumberTypeDefinition() const;
			VariableTableItem* FindVariable(std::string const& variableName);
		};
	}
}