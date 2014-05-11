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
#include "ScriptParameters.hpp"

namespace geogen
{
	namespace corelib
	{
		class BooleanTypeDefinition;
		class NumberTypeDefinition;
	}

	namespace runtime
	{
		class ManagedObject;
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
			
			// Memory manager must be created first and destroyed last.
			MemoryManager memoryManager;

			ObjectStack objectStack;
			CallStack callStack;

			CompiledScript const& compiledScript;

			VariableTable globalVariableTable;
			ScriptParameters arguments;

			void InitializeTypes();
			void InitializeGlobalVariables();
			void InitializeMainFunction();
			void ValidateArguments();

			// Non-copyable
			VirtualMachine(VirtualMachine const&) : globalVariableTable(NULL), compiledScript(compiledScript) {};
			VirtualMachine& operator=(VirtualMachine const&) {};
		public:
			VirtualMachine(CompiledScript const& compiledScript, ScriptParameters const& arguments);
			~VirtualMachine() {};

			inline VirtualMachineStatus GetStatus() const { return this->status; }
			inline MemoryManager& GetMemoryManager() { return this->memoryManager; }
			inline VariableTable& GetGlobalVariableTable() { return this->globalVariableTable; }
			inline ScriptParameters const& GetArguments() { return this->arguments; }

			inline CompiledScript const& GetCompiledScript() const { return this->compiledScript; };

			inline ObjectStack& GetObjectStack() { return this->objectStack; };
			//inline std::stack<ManagedObject const*> const& GetObjectStack() const { return *((std::stack<ManagedObject const*>*)&this->objectStack); };

			inline CallStack& GetCallStack() { return this->callStack; };
			//inline std::stack<CallStackEntry const> const& GetCallStack() const { return *((std::stack<ManagedObject const*>*)&this->callStack); };

			VirtualMachineStepResult Step();

			void CallFunction(CodeLocation location, FunctionDefinition const* functionDefintion, ManagedObject* instance, unsigned numberOfArguments);

			void Run();

			ManagedObject* GetNull();
			TypeDefinition const* GetTypeDefinition(std::string const& typeName) const;
			corelib::BooleanTypeDefinition const* GetBooleanTypeDefinition() const;
			corelib::NumberTypeDefinition const* GetNumberTypeDefinition() const;
			VariableTableItem* FindVariable(std::string const& variableName);
			ManagedObject* GetStaticInstance(std::string const& typeName);
		};
	}
}