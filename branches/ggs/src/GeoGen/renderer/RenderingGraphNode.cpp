#include "RenderingGraphNode.hpp"
#include "RenderingStep.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;

RenderingGraphNode::RenderingGraphNode(RenderingStep const* step) : step(step), backEdges(step->GetArgumentSlots().size(), NULL)
{
}


void RenderingGraphNode::AddEdge(RenderingGraphNode* target)
{
	this->edges.push_back(target); 

	unsigned backEdgeMatchCount = 0;
	unsigned i = 0;
	for (vector<unsigned>::const_iterator it = target->GetStep()->GetArgumentSlots().begin(); it != target->GetStep()->GetArgumentSlots().end(); it++)
	{
		if (this->GetStep()->GetReturnSlot() == *it)
		{
			target->backEdges[i] = this;
			backEdgeMatchCount++;
		}

		i++;
	}	

	if (backEdgeMatchCount == 0)
	{
		throw InternalErrorException(GG_STR("No rendering graph back edge match."));
	}
}

