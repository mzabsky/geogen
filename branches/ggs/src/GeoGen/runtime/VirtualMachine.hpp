#pragma once

#include <vector>
#include <string>
#include <stack>

#include "CompiledScript.hpp"
#include "CallStackEntry.hpp"
#include "MemoryManager.hpp"

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;

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
			
			std::stack<DynamicObject*> objectStack;
			std::stack<CallStackEntry> callStack;

			CompiledScript const& compiledScript;

			MemoryManager memoryManager;
		public:
			VirtualMachine(CompiledScript const& compiledScript)
				: compiledScript(compiledScript), status(VIRTUAL_MACHINE_STATUS_READY) {};
			~VirtualMachine() {};

			VirtualMachineStatus GetStatus() const { return this->status; }
			MemoryManager& GetMemoryManager() { return this->memoryManager; }

			inline CompiledScript const& GetCompiledScript() const { return this->compiledScript; };

			inline std::stack<DynamicObject*>& GetObjectStack() { return this->objectStack; };
			inline std::stack<DynamicObject const*> const& GetObjectStack() const { return *((std::stack<DynamicObject const*>*)&this->objectStack); };

			VirtualMachineStepResult Step();
		};
	}
}