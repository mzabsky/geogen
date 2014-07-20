#include "Renderer.hpp"
#include "../InternalErrorException.hpp"
#include "RenderingStep.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;

Renderer::Renderer(RenderingSequence const& renderingSequence)
: renderingSequence(renderingSequence), objectTable(renderingSequence.GetRequiredObjectTableSize()), status(RENDERER_STATUS_READY), renderingSequenceMetadata(renderingSequence), graph(renderingSequence)
{
}

void Renderer::Run()
{
	while (this->status == RENDERER_STATUS_READY)
	{
		this->Step();
	}
}

void Renderer::CalculateRenderingBounds()
{
	vector<RenderingBounds*> renderingBoundsBySlot(this->renderingSequence.GetRequiredObjectTableSize(), NULL);

	for (RenderingSequence::const_reverse_iterator it = this->renderingSequence.RBegin(); it != this->renderingSequence.REnd(); it++)
	{
		vector<RenderingBounds*> preparedBounds;
		RenderingGraphNode* node = this->graph.GetNodeByStep(*it);
		for (RenderingGraphNode::iterator it2 = node->BackBegin(); it2 != node->BackEnd(); it2++)
		{
			preparedBounds.push_back(this->GetRenderingSequenceMetadata().GetRenderingBounds((*it2)->GetStep()));
		}

		(*it)->UpdateRenderingBounds(this, preparedBounds);
	}
}