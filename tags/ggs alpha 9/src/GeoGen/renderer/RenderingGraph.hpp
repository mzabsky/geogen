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