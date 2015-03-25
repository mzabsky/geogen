/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <vector>
#include <map>

#include <GeoGen/GeoGen.hpp>
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

			unsigned numberOfShownLines;

			bool aborted;

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
			inline runtime::VirtualMachine const* GetVirtualMachine() const { return &this->vm; };

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