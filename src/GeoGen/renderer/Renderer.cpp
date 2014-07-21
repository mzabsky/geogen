#include "Renderer.hpp"
#include "../InternalErrorException.hpp"
#include "RenderingStep.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;

const String Renderer::MAP_NAME_MAIN = GG_STR("main");

Renderer::Renderer(RenderingSequence const& renderingSequence)
: renderingSequence(renderingSequence), nextStep(renderingSequence.Begin()), objectTable(renderingSequence.GetRequiredObjectTableSize()), status(RENDERER_STATUS_READY), renderingSequenceMetadata(renderingSequence), graph(renderingSequence)
{
}

RendererStepResult Renderer::Step()
{
	if (this->nextStep == this->renderingSequence.End())
	{
		this->status = RENDERER_STATUS_FINISHED;
		return RENDERER_STEP_RESULT_FINISHED;
	}

	(*this->nextStep)->Step(this);
	this->nextStep++;

	if (this->nextStep == this->renderingSequence.End())
	{
		this->status = RENDERER_STATUS_FINISHED;
		return RENDERER_STEP_RESULT_FINISHED;
	}

	return RENDERER_STEP_RESULT_RUNNING;
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