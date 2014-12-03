#pragma once

#include <vector>
#include <stack>

#include "../String.hpp"
#include "CompiledScript.hpp"
#include "CallStackEntry.hpp"
#include "MemoryManager.hpp"
#include "VariableTable.hpp"
#include "CallStack.hpp"
#include "ObjectStack.hpp"
#include "ScriptParameters.hpp"
#include "../renderer/RenderingSequence.hpp"
#include "../renderer/RendererObjectSlotTable.hpp"
#include "../random/RandomSequence.hpp"

namespace geogen
{
	namespace corelib
	{
		class BooleanTypeDefinition;
		class NumberTypeDefinition;
	}

	namespace renderer
	{
		class RenderingStep;
	}

	namespace runtime
	{
		class ManagedObject;
		class VariableTableItem;

		/// Statuses of the VirtualMachine.
		enum VirtualMachineStatus
		{
			/// The VM is ready to execute.
			VIRTUAL_MACHINE_STATUS_READY,

			/// Execution of the virtual machine was finished successfully, no further execution is possible.
			VIRTUAL_MACHINE_STATUS_FINISHED,

			/// Execution of the virtual machine was terminated prematurely with an error, no further execution is possible.
			VIRTUAL_MACHINE_STATUS_FAULTED
		};

		/// Results of VirtualMachine::Step. There is no "failed" result, because that is indicated by throwing exception.
		enum VirtualMachineStepResult
		{
			VIRTUAL_MACHINE_STEP_RESULT_RUNNING,
			VIRTUAL_MACHINE_STEP_RESULT_FINISHED
		};		

		/// Intermediate code simulator. Loads a CompiledScript and and then executes the script, producing a geogen::renderer::RenderingSequence.
		class VirtualMachine : public Serializable
		{
		public:
			/// Sript message handler callback. Takes reference to the VirtualMachine, location where the Print/TriggerError function was called and formatted and unformatted message, along with its formatting arguments.
			/// @see @ref tutorial_text_messages
			typedef void(*ScriptMessageHandler)(VirtualMachine* virtualMachine, CodeLocation location, String const& formattedMessage, String const& unformattedMessage, std::vector<String> arguments);
		private:
			VirtualMachineStatus status;
			unsigned instructionCounter = 0;

			// Memory manager must be created first and destroyed last.
			MemoryManager memoryManager;

			ObjectStack objectStack;
			CallStack callStack;

			CompiledScript const& compiledScript;

			VariableTable globalVariableTable;
			ScriptParameters arguments;

			ScriptMessageHandler scriptMessageHandler;

			renderer::RendererObjectSlotTable rendererObjectSlotTable;
			renderer::RenderingSequence renderingSequence;

			void InitializeTypes();
			void InitializeGlobalVariables();
			void InitializeMainFunction();
			void ValidateArguments();

			// Non-copyable
			VirtualMachine(VirtualMachine const&) : globalVariableTable(NULL), compiledScript(compiledScript), scriptMessageHandler(DefaultScriptMessageHandler), renderingSequence(0), commonRandomSequence(0) {};
			VirtualMachine& operator=(VirtualMachine const&) {};

			void* callbackData = NULL;

			random::RandomSequence commonRandomSequence;

			std::vector<String> generatedMaps;

			void Finish();
		public:
			/// Default (empty) script parameters obeject.
			static const ScriptParameters SCRIPT_PARAMETERS_DEFAULT;

			/// Constructor.
			/// @param compiledScript The compiled script. Does not assume ownership of this pointer (it will no be destroyed when the VM is destroyed).
			/// @param arguments The arguments. Creates copy of this object.
			VirtualMachine(CompiledScript const& compiledScript, ScriptParameters const& arguments = SCRIPT_PARAMETERS_DEFAULT);	
			
			/// Destructor. Releases all owned objects.
			~VirtualMachine() {};

			/// Gets current status.
			/// @return The status.
			inline VirtualMachineStatus GetStatus() const { return this->status; }

			/// Gets memory manager.
			/// @return The memory manager.
			inline MemoryManager& GetMemoryManager() { return this->memoryManager; }

			/// Gets global variable table.
			/// @return The global variable table.
			inline VariableTable& GetGlobalVariableTable() { return this->globalVariableTable; }

			/// Gets the arguments (copy of the ScriptParameters object assigned to the machine upon construction).
			/// @return The arguments.
			inline ScriptParameters const& GetArguments() { return this->arguments; }

			/// Gets compiled script.
			/// @return The compiled script.
			inline CompiledScript const& GetCompiledScript() const { return this->compiledScript; };

			/// Gets current script message handler. If no handler was assigned previously to this VM, DefaultScriptMessageHandler is returned.
			/// @return The script message handler.
			/// @see @ref tutorial_text_messages
			inline ScriptMessageHandler GetScriptMessageHandler() const { return this->scriptMessageHandler; };

			/// Sets script message handler.
			/// @param scriptMessageHandler The script message handler.
			/// @see @ref tutorial_text_messages
			inline void SetScriptMessageHandler(ScriptMessageHandler scriptMessageHandler) { this->scriptMessageHandler = scriptMessageHandler; };

			/// Default script message handler. Prints the script message to standard output.
			/// @param vm The virtual machine.
			/// @param location The code location.
			/// @param formattedMessage The formatted message.
			/// @param unformattedMessage The unformattedMessage.
			/// @param arguments The formatting arguments.
			/// @see @ref tutorial_text_messages
			static void DefaultScriptMessageHandler(VirtualMachine* vm, CodeLocation location, String const& formattedMessage, String const& unformattedMessage, std::vector<String> arguments);

			/// Empty script message handler. Discards the script message.
			/// @param vm The virtual machine.
			/// @param location The code location.
			/// @param formattedMessage The formatted message.
			/// @param unformattedMessage The unformattedMessage.
			/// @param arguments The formatting arguments.
			/// @see @ref	tutorial_text_messages
			static void EmptyScriptMessageHandler(VirtualMachine* vm, CodeLocation location, String const& formattedMessage, String const& unformattedMessage, std::vector<String> arguments);

			/// Gets object stack.
			/// @return The object stack.
			inline ObjectStack& GetObjectStack() { return this->objectStack; };

			/// Gets object stack.
			/// @return The object stack.
			inline ObjectStack const& GetObjectStack() const { return this->objectStack; };
			//inline std::stack<ManagedObject const*> const& GetObjectStack() const { return *((std::stack<ManagedObject const*>*)&this->objectStack); };

			/// Gets call stack.
			/// @return The call stack.
			inline CallStack& GetCallStack() { return this->callStack; };

			/// Gets call stack.
			/// @return The call stack.
			inline CallStack const& GetCallStack() const { return this->callStack; };
			//inline std::stack<CallStackEntry const> const& GetCallStack() const { return *((std::stack<ManagedObject const*>*)&this->callStack); };

			/// Gets reference to the generated rendering sequence.
			/// @return The rendering sequence.
			inline renderer::RenderingSequence& GetRenderingSequence() { return this->renderingSequence; }
			
			/// Gets reference to the generated rendering sequence.
			/// @return The rendering sequence.
			inline renderer::RenderingSequence const& GetRenderingSequence() const { return this->renderingSequence; }

			/// Gets renderer object slot table. Used to assign renderer object slots to managed objects.
			/// @return The renderer object slot table.
			inline renderer::RendererObjectSlotTable& GetRendererObjectSlotTable() { return this->rendererObjectSlotTable; }

			/// Gets renderer object slot table. Used to assign renderer object slots to managed objects.
			/// @return The renderer object slot table.
			inline renderer::RendererObjectSlotTable const& GetRendererObjectSlotTable() const { return this->rendererObjectSlotTable; }

			/// Advances the execution by a single step.
			/// @return A step result.
			VirtualMachineStepResult Step();

			/// Pushes a function call frame onto an appropriate place in the call stack.
			/// @param location The code location.
			/// @param functionDefintion The function defintion.
			/// @param instance Instance, if the function is a member method, null otherwise.
			/// @param numberOfArguments Actual number of arguments to the call.
			void CallFunction(CodeLocation location, FunctionDefinition const* functionDefintion, ManagedObject* instance, unsigned numberOfArguments);

			unsigned GetInstructionCounter() const { return this->instructionCounter; }

			/// Gets common random sequence.
			/// @return The common random sequence.
			random::RandomSequence& GetCommonRandomSequence() { return this->commonRandomSequence; }

			/// Gets a list of random map names that were yielded so far.
			/// @return The generated map names.
			std::vector<String>& GetGeneratedMaps() { return this->generatedMaps; };
			
			/// Gets a list of random map names that were yielded so far.
			/// @return The generated map names.
			std::vector<String> const& GetGeneratedMaps() const { return this->generatedMaps; };

			/// Advances the execution until finish or failure.
			void Run();

			/// Gets the managed object representing null.
			/// @return null The null managed object.
			ManagedObject* GetNull();

			/// Gets type definition by its name. Triggers runtime error if not found.
			/// @param typeName Name of the type.
			/// @return The type definition.
			TypeDefinition const* GetTypeDefinition(String const& typeName) const;
			
			/// Gets type definition of the Boolean type. Triggers runtime error if not found.
			/// @return The type definition.
			corelib::BooleanTypeDefinition const* GetBooleanTypeDefinition() const;
			
			/// Gets type definition of the Number type. Triggers runtime error if not found.
			/// @return The type definition.
			corelib::NumberTypeDefinition const* GetNumberTypeDefinition() const;

			/// Searches for the first variable matching @a variableName. THe search will go from the inmost entry of the code block stack to the outermost and then will check global variables.
			/// @param variableName Name of the variable.
			/// @return Variable table item if found, null otherwise.
			VariableTableItem* FindVariable(String const& variableName);

			/// Gets managed object representing type matching @a typeName.
			/// @param typeName Name of the type.
			/// @return Managed object if found, null otherwise.
			ManagedObject* GetStaticInstance(String const& typeName);

			/// Gets callback data.
			/// @return Callback data pointer if set, null otherwise.
			inline void* GetCallbackData() const { return this->callbackData; }

			/// Sets the callback data pointer. 
			/// @param callbackData The callback data pointer. The VM does not assume ownership of this pointer.
			inline void SetCallbackData(void* callbackData) { this->callbackData = callbackData; }

			/// Adds a step to the rendering sequence. Throws overflow exception if too large.
			/// @param location The code location.
			/// @param renderingStep The rendering step.
			void AddRenderingStep(CodeLocation location, renderer::RenderingStep* renderingStep);

			virtual void Serialize(IOStream& stream) const;
		};
	}
}