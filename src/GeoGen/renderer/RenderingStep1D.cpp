#include "../InternalErrorException.hpp"
#include "RenderingStep1D.hpp"
#include "RenderingBounds1D.hpp"
#include "Renderer.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;

void RenderingStep1D::SetRenderingBounds(Renderer* renderer, Interval interval) const
{
	dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->SetInterval(interval);
}

void RenderingStep1D::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	if (argumentBounds.size() == 0)
	{
		this->TriggerRenderingBoundsCalculationError("empty referencing steps list");
	}

	Interval newInterval;
	for (std::vector<RenderingBounds*>::iterator it = argumentBounds.begin(); it != argumentBounds.end(); it++)
	{
		if ((*it)->GetRenderingStepType() != RENDERING_STEP_TYPE_1D)
		{
			this->TriggerRenderingBoundsCalculationError("incompatible rendering step type");
		}

		RenderingBounds1D* current = dynamic_cast<RenderingBounds1D*>(*it);
		
		if (it == argumentBounds.begin())
		{
			newInterval = current->GetInterval();
		}
		else {
			newInterval = Interval::Combine(newInterval, current->GetInterval());
		}
	}

	this->SetRenderingBounds(renderer, newInterval);
}