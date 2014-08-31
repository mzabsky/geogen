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
			static const unsigned NUMBER_OF_SHOWN_LINES_DEFAULT;

			runtime::VirtualMachine vm;
			CommandTable commandTable;

			IStream& in;
			OStream& out;

			String outputDirectory;

			unsigned numberOfShownLines = NUMBER_OF_SHOWN_LINES_DEFAULT;

			bool aborted = false;

			// Non-copyable
			Debugger(Debugger const&) : vm(*(runtime::CompiledScript*)NULL, *(runtime::ScriptParameters*)NULL), in(Cin), out(Cout) {};
			Debugger& operator=(Debugger const&) {};
		public:
			Debugger(IStream& in, OStream& out, runtime::CompiledScript const& code, runtime::ScriptParameters parameters, String outputDirectory);
			~Debugger() {};

			inline IStream& GetIn() { return this->in; }
			inline OStream& GetOut() { return this->out; }

			inline String GetOutputDirectory() { return this->outputDirectory; }

			inline runtime::VirtualMachine* GetVirtualMachine() { return &this->vm; };
			runtime::instructions::Instruction const* GetCurrentInstruction() const;
			inline CommandTable& GetCommandTable() { return this->commandTable; };

			void ShowCodeContext() const;
			inline unsigned GetNumberOfShownLines() const { return this->numberOfShownLines; }
			inline void SetNumberOfShownLines(unsigned numberOfShownLines) { this->numberOfShownLines = numberOfShownLines; }

			inline void Abort() { this->aborted = true; }
			inline bool IsAborted() const { return this->aborted; }

			void Run();
		};
	}
}