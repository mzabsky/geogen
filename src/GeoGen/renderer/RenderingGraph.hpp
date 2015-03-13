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

#include "RenderingSequence.hpp"
#include "RenderingGraphNode.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingSequenceStep;

		/// A graph of dependenies between RenderingStep objects.
		class RenderingGraph
		{
		private:
			RenderingSequence const& renderingSequence;
			std::map<RenderingStep const*, RenderingGraphNode> nodes;
			std::vector<RenderingGraphNode*> entryNodes;
		public:

			/// Initializes instance of the RenderingGraph class.
			/// @param renderingSequence The rendering sequence. The sequence must exist for entire duration of life of this object. 
			RenderingGraph(RenderingSequence const& renderingSequence);

			/// Gets RenderingGraphNode corresponding to a specific RenderingStep.
			/// @param step The rendering step.
			/// @return The node.
			RenderingGraphNode* GetNodeByStep(RenderingStep const* step);
		};
	}
}