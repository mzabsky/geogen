#include <map>

#include "RenderingGraph.hpp"
#include "RenderingStep.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;

RenderingGraph::RenderingGraph(RenderingSequence const& renderingSequence) : renderingSequence(renderingSequence)
{
	map<unsigned, vector<RenderingGraphNode*>> openNodes;
	for (RenderingSequence::const_reverse_iterator it = renderingSequence.RBegin(); it != renderingSequence.REnd(); it++)
	{
		RenderingStep const* step = *it;
		this->nodes.insert(pair<RenderingStep const*, RenderingGraphNode>(step, RenderingGraphNode(step)));
		RenderingGraphNode* node = &this->nodes[step];

		if (openNodes.find(step->GetReturnSlot()) == openNodes.end())
		{		
			this->entryNodes.push_back(node);
		}
		else
		{
			vector<RenderingGraphNode*> foundRange = openNodes.find(step->GetReturnSlot())->second;
			for (vector<RenderingGraphNode*>::iterator it2 = foundRange.begin(); it2 != foundRange.end(); it2++)
			{
				node->AddEdge(*it2);
			}

			openNodes.erase(step->GetReturnSlot());
		}

		for (vector<unsigned>::const_iterator it2 = step->GetArgumentSlots().begin(); it2 != step->GetArgumentSlots().end(); it2++)
		{
			openNodes[*it2].push_back(node);
		}
	}

	if (openNodes.size() > 0)
	{
		throw InternalErrorException(GG_STR("Graph calculation consistency error."));
	}
}

RenderingGraphNode* RenderingGraph::GetNodeByStep(RenderingStep const* step)
{
	return &this->nodes[step];
}