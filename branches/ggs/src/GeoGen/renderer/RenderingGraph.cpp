#include <map>

#include "RenderingGraph.hpp"
#include "RenderingStep.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;

RenderingGraph::RenderingGraph(RenderingSequence const& renderingSequence) : renderingSequence(renderingSequence)
{
	multimap<unsigned, RenderingGraphNode*> openNodes;
	for (RenderingSequence::const_reverse_iterator it = renderingSequence.RBegin(); it != renderingSequence.REnd(); it++)
	{
		RenderingStep const* step = *it;
		this->nodes.insert(pair<RenderingStep const*, RenderingGraphNode>(step, RenderingGraphNode(step)));
		RenderingGraphNode* node = &this->nodes[step];

		if (openNodes.count(step->GetReturnSlot()) == 0)
		{		
			this->entryNodes.push_back(node);
		}
		else
		{
			pair<multimap<unsigned, RenderingGraphNode*>::iterator, multimap<unsigned, RenderingGraphNode*>::iterator> foundRange = openNodes.equal_range(step->GetReturnSlot());
			for (multimap<unsigned, RenderingGraphNode*>::iterator it2 = foundRange.first; it2 != foundRange.second; it2++)
			{
				node->AddEdge(it2->second);
				//it2->second->AddEdge(node);
			}

			openNodes.erase(foundRange.first, foundRange.second);
		}

		for (vector<unsigned>::const_iterator it2 = step->GetArgumentSlots().begin(); it2 != step->GetArgumentSlots().end(); it2++)
		{
			openNodes.insert(pair<unsigned, RenderingGraphNode*>(*it2, node));
		}
		/*
		if (openNodes.count(step->GetReturnSlot()) == 0)
		{
			openNodes.erase(step->GetReturnSlot());
			this->entryNodes.push_back(node);
		}
		else 
		{
			pair<multimap<unsigned, RenderingGraphNode*>::iterator, multimap<unsigned, RenderingGraphNode*>::iterator> foundRange = openNodes.equal_range(step->GetReturnSlot());
			for (multimap<unsigned, RenderingGraphNode*>::iterator it2 = foundRange.first; it2 != foundRange.second; it2++)
			{
				it2->second->AddEdge(node);
			}

			openNodes.erase(foundRange.first, foundRange.second);
		}

		openNodes.insert(pair<unsigned, RenderingGraphNode*>(step->GetReturnSlot(), node));

		for (vector<unsigned>::const_iterator it2 = step->GetArgumentSlots().begin(); it2 != step->GetArgumentSlots().end(); it2++)
		{
			openNodes.insert(pair<unsigned, RenderingGraphNode*>(*it2, node));
		}*/
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