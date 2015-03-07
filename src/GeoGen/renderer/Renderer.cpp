#include <numeric>

#include "Renderer.hpp"
#include "../InternalErrorException.hpp"
#include "RenderingStep.hpp"
#include "RenderingBounds.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;

const String Renderer::MAP_NAME_MAIN = GG_STR("main");

Renderer::Renderer(RenderingSequence const& renderingSequence, Configuration configuration)
: configuration(configuration), renderingSequence(renderingSequence), nextStep(renderingSequence.Begin()), objectTable(renderingSequence.GetRequiredObjectTableSize()), status(RENDERER_STATUS_READY), renderingSequenceMetadata(renderingSequence), graph(renderingSequence)
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
	this->CalculateMemoryRequirements();
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

void Renderer::CalculateMemoryRequirements()
{
	vector<unsigned> allocatedMemoryPerSlot(this->GetObjectTable().GetSize(), 0);
	vector<bool> isObjectAlive(this->GetObjectTable().GetSize(), false);
	vector<RenderingBounds const*> currentBounds(this->GetObjectTable().GetSize(), NULL);

	for (RenderingSequence::const_iterator it = this->renderingSequence.Begin(); it != this->renderingSequence.End(); it++)
	{
		RenderingStep const* step = *it;

		unsigned returnSlot = step->GetReturnSlot();
		vector<unsigned>& currentStepObjectIndexesToRelease = this->GetRenderingSequenceMetadata().GetObjectIndexesToRelease(step);

		// Memory already allocated by renderer objects
		unsigned currentAllocatedSum = std::accumulate(allocatedMemoryPerSlot.begin(), allocatedMemoryPerSlot.end(), 0);

		// Prepare list of argument bounds for extra memory calculation
		vector<RenderingBounds const*> argumentBounds;
		for (vector<unsigned>::const_iterator it2 = step->GetArgumentSlots().begin(); it2 != step->GetArgumentSlots().end(); it2++)
		{
			argumentBounds.push_back(currentBounds[*it2]);
		}

		// Memory that will be required by this step beyond the memory allocated by pre-existing renderer objects
		unsigned stepExtraMemory = (*it)->GetPeakExtraMemory(this, argumentBounds);

		this->GetRenderingSequenceMetadata().SetMemoryRequirement(step, currentAllocatedSum + stepExtraMemory);

		// If an object is being allocated, store its size
		if (!isObjectAlive[returnSlot]){

			// This is done only once for each object, because the bounds will not be resized even if later operations have smaller bounds
			// (they can't have larger bounds).
			currentBounds[returnSlot] = this->GetRenderingSequenceMetadata().GetRenderingBounds(*it);
			allocatedMemoryPerSlot[returnSlot] = currentBounds[returnSlot]->GetMemorySize();
			isObjectAlive[returnSlot] = true;

			// TODO: Co s Rescale?
			// TODO: Fyzicke koordinaty
		}

		// Released objects don't occupy any memory any more
		for (vector<unsigned>::iterator it2 = currentStepObjectIndexesToRelease.begin(); it2 != currentStepObjectIndexesToRelease.end(); it2++)
		{
			allocatedMemoryPerSlot[*it2] = 0;
			isObjectAlive[*it2] = false;
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