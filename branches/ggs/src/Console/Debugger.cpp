#include "Debugger.hpp"
#include "runtime_commands/CallStackRuntimeCommand.hpp"
#include "runtime_commands/CodeBlockCodeRuntimeCommand.hpp"
#include "runtime_commands/CodeBlockStackRuntimeCommand.hpp"
#include "runtime_commands/HelpRuntimeCommand.hpp"
#include "runtime_commands/ManagedObjectsRuntimeCommand.hpp"
#include "runtime_commands/ObjectStackRuntimeCommand.hpp"
#include "runtime_commands/ReadVariableRuntimeCommand.hpp"
#include "runtime_commands/RenderingSequenceRuntimeCommand.hpp"
#include "runtime_commands/RunRuntimeCommand.hpp"
#include "runtime_commands/StepRuntimeCommand.hpp"

using namespace geogen;
using namespace compiler;
using namespace runtime;
using namespace console;
using namespace renderer;
using namespace utils;
using namespace std;
using namespace runtime_commands;
using namespace instructions;

Debugger::Debugger(IStream& in, OStream& out, runtime::CompiledScript const& code, runtime::ScriptParameters parameters)
: vm(code, parameters), in(in), out(out)
{
	commandTable.AddCommand(new CallStackRuntimeCommand());
	commandTable.AddCommand(new CodeBlockCodeRuntimeCommand());
	commandTable.AddCommand(new CodeBlockStackRuntimeCommand());
	commandTable.AddCommand(new HelpRuntimeCommand());
	commandTable.AddCommand(new ManagedObjectsRuntimeCommand());
	commandTable.AddCommand(new ObjectStackRuntimeCommand());
	commandTable.AddCommand(new ReadVariableRuntimeCommand());
	commandTable.AddCommand(new RenderingSequenceRuntimeCommand());
	commandTable.AddCommand(new RunRuntimeCommand());
	commandTable.AddCommand(new StepRuntimeCommand());
}

runtime::instructions::Instruction const* Debugger::GetCurrentInstruction()
{
	if (this->GetVirtualMachine() == NULL) return NULL;
	if (this->GetVirtualMachine()->GetCallStack().IsEmpty()) return NULL;

	runtime::CallStackEntry& callStackTop = this->GetVirtualMachine()->GetCallStack().Top();

	if (callStackTop.GetCodeBlockStack().IsEmpty()) return NULL;

	runtime::CodeBlockStackEntry& codeBlockStackTop = callStackTop.GetCodeBlockStack().Top();

	return codeBlockStackTop.GetCurrentInstruction();
};

void Debugger::Run()
{
	String input = "";
	while (vm.GetStatus() == VIRTUAL_MACHINE_STATUS_READY)
	{
		Instruction const* currentInstruction = this->GetCurrentInstruction();
		out << endl << GG_STR("Next instruction: ") << (currentInstruction->ToString()) << GG_STR(" on line ") << currentInstruction->GetLocation().GetLine() << GG_STR(", column ") << currentInstruction->GetLocation().GetColumn() << GG_STR(". ") << endl << endl << GG_STR("Command? ");

		getline<Char>(in, input);

		out << "Input: " << input << endl << endl;

		size_t separatorPosition = input.find(" ");
		string commandCue = input.substr(0, separatorPosition);
		string args = "";
		if (separatorPosition != String::npos)
		{
			args = input.substr(separatorPosition + 1);
		}

		Command const* command = this->commandTable.GetCommand(commandCue);
		if (command == NULL)
		{
			out << GG_STR("Unknown command");
		}
		else
		{
			dynamic_cast<RuntimeCommand const*>(command)->Run(this, args);
		}
	}
}