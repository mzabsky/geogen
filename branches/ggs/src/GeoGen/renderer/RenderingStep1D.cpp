#include "../InternalErrorException.hpp"
#include "RenderingStep1D.hpp"
#include "RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;

void RenderingStep1D::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds)
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
}