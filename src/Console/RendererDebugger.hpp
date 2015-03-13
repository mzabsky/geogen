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
		class RendererDebugger
		{
			renderer::Renderer renderer;
			CommandTable commandTable;

			IStream& in;
			OStream& out;

			String outputDirectory;

			bool aborted = false;
			bool autosave = false;

			unsigned currentStepNumber = 0;

			double lastStepTime = 0;

			// Non-copyable
			RendererDebugger(RendererDebugger const&) : renderer(*(renderer::RenderingSequence*)NULL), in(Cin), out(Cout) {};
			RendererDebugger& operator=(RendererDebugger const&) {};
		public:
			RendererDebugger(geogen::IStream& in, geogen::OStream& out, renderer::RenderingSequence const& sequence, String outputDirectory);
			~RendererDebugger() {};

			inline IStream& GetIn() { return this->in; }
			inline OStream& GetOut() { return this->out; }

			inline String GetOutputDirectory() { return this->outputDirectory; }

			inline renderer::Renderer* GetRenderer() { return &this->renderer; };
			inline CommandTable& GetCommandTable() { return this->commandTable; };

			inline void Abort() { this->aborted = true; }
			inline bool IsAborted() const { return this->aborted; }

			inline void SetAutosave(bool autosave) { this->autosave = autosave; }
			inline bool GetAutosave() const { return this->autosave; }

			inline double GetLastStepTime() const { return this->lastStepTime; }

			void Run();

			void Step();
		};
	}
}