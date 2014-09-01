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

	// Release objects that won't be required by any future steps
	vector<unsigned> const& objectsToRelease = this->GetRenderingSequenceMetadata().GetObjectIndexesToRelease(*this->nextStep);
	for (vector<unsigned>::const_iterator it = objectsToRelease.begin(); it != objectsToRelease.end(); it++)
	{
		this->GetObjectTable().ReleaseObject(*it);
	}
	
	this->nextStep++;
	this->stepCounter++;

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

void Renderer::CalculateMetadata()
{
	this->CalculateRenderingBounds();
	this->CalculateObjectLifetimes();
}

void Renderer::CalculateRenderingBounds()
{
	for (RenderingSequence::const_reverse_iterator it = this->renderingSequence.RBegin(); it != this->renderingSequence.REnd(); it++)
	{
		vector<RenderingBounds*> argumentBounds;
		RenderingGraphNode* node = this->graph.GetNodeByStep(*it);
		for (RenderingGraphNode::iterator it2 = node->BackBegin(); it2 != node->BackEnd(); it2++)
		{
			argumentBounds.push_back(this->GetRenderingSequenceMetadata().GetRenderingBounds((*it2)->GetStep()));
		}

		(*it)->UpdateRenderingBounds(this, argumentBounds);
	}
}

void Renderer::CalculateObjectLifetimes()
{
	vector<bool> isObjectAlive(this->GetObjectTable().GetSize(), true);

	for (RenderingSequence::const_reverse_iterator it = this->renderingSequence.RBegin(); it != this->renderingSequence.REnd(); it++)
	{
		vector<unsigned>& currentStepObjectIndexesToRelease = this->GetRenderingSequenceMetadata().GetObjectIndexesToRelease(*it);

		for (vector<unsigned>::const_iterator it2 = (*it)->GetArgumentSlots().begin(); it2 != (*it)->GetArgumentSlots().end(); it2++)
		{
			if (isObjectAlive[*it2])
			{
				currentStepObjectIndexesToRelease.push_back(*it2);
				isObjectAlive[*it2] = false;
			}
		}
	}
}

double Renderer::GetProgress() const
{
	return stepCounter * 100 / double(this->GetRenderingSequence().Size());
}

void Renderer::Serialize(IOStream& stream) const
{
	stream << "Status: " << status << endl;
	stream << "StepCounter: " << stepCounter << endl;

	stream << "RenderingSequence: " << endl;
	this->renderingSequence.SerializeWithTabs(stream, 1);

	stream << "ObjectTable: " << endl;
	this->objectTable.SerializeWithTabs(stream, 1);

	stream << "RenderingSequenceMetadata: " << endl;
	this->renderingSequenceMetadata.SerializeWithTabs(stream, 1);

	/*stream << "RenderingSequenceGraph: " << endl;
	this->graph.SerializeWithTabs(stream, 1);*/

	stream << "RenderedMapTable: " << endl;
	this->renderedMapTable.SerializeWithTabs(stream, 1);
}