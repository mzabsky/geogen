#pragma once

#include <vector>
#include <map>

#include "../GeoGen/GeoGen.hpp"
#include "CommandTable.hpp"

namespace geogen
{
	namespace console
	{
		class Debugger
		{
			runtime::VirtualMachine vm;
			CommandTable commandTable;

			IStream& in;
			OStream& out;

			// Non-copyable
			Debugger(Debugger const&) : vm(*(runtime::CompiledScript*)NULL, *(runtime::ScriptParameters*)NULL), in(Cin), out(Cout) {};
			Debugger& operator=(Debugger const&) {};
		public:
			Debugger(IStream& in, OStream& out, runtime::CompiledScript const& code, runtime::ScriptParameters parameters);
			~Debugger() {};

			inline IStream& GetIn() { return this->in; }
			inline OStream& GetOut() { return this->out; }

			inline runtime::VirtualMachine* GetVirtualMachine() { return &this->vm; };
			runtime::instructions::Instruction const* GetCurrentInstruction();
			inline CommandTable& GetCommandTable() { return this->commandTable; };

			void Run();
		};
	}
}