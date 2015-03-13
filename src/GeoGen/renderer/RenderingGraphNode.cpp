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

