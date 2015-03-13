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

#include "RenderingSequence.hpp"
#include "../CodeLocation.hpp"
#include "../Point.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingBounds;
		class RenderingStep;

		class RenderingSequenceMetadata : public Serializable
		{
		private:
			std::map<RenderingStep const*, unsigned> stepNumbers;
			std::vector<RenderingBounds*> renderingBounds;
			std::vector<std::vector<unsigned> > objectsIndexesToRelease;
			std::vector<unsigned> memoryRequirements;
			
			// Non-copyable
			RenderingSequenceMetadata(RenderingSequenceMetadata const&) {};
			RenderingSequenceMetadata& operator=(RenderingSequenceMetadata const&) {};
		protected:
		public:
			RenderingSequenceMetadata(RenderingSequence const& renderingSequence);
			~RenderingSequenceMetadata();

			RenderingBounds* GetRenderingBounds(RenderingStep const* step);

			std::vector<unsigned>& GetObjectIndexesToRelease(RenderingStep const* step);
			std::vector<unsigned> const& GetObjectIndexesToRelease(RenderingStep const* step) const;
			unsigned GetMemoryRequirement(RenderingStep const* step) const;
			void SetMemoryRequirement(RenderingStep const* step, unsigned memoryRequirement);

			unsigned GetStepNumberByAddress(RenderingStep const* step) const;

			virtual void Serialize(IOStream& stream) const;
		};
	}
}