#pragma once

#include <vector>

#include "RenderingSequence.hpp"
#include "RenderingGraphNode.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingSequenceStep;

		class RenderingGraph
		{
		private:
			RenderingSequence const& renderingSequence;
			std::map<RenderingStep const*, RenderingGraphNode> nodes;
			std::vector<RenderingGraphNode*> entryNodes;
		public:
			RenderingGraph(RenderingSequence const& renderingSequence);

			RenderingGraphNode* GetNodeByStep(RenderingStep const* step);
		};
	}
}