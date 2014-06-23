#include "../InternalErrorException.hpp"
#include "RenderingStep1D.hpp"

using namespace geogen;
using namespace renderer;

void RenderingStep1D::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingStep*> referencingSteps)
{
	if (referencingSteps.size() == 0)
	{
		this->TriggerRenderingBoundsCalculationError("empty referencing steps list");
	}

	Coordinate newRenderOrigin;
	Size1D newRenderSize;
	for (std::vector<RenderingStep*>::iterator it = referencingSteps.begin(); it != referencingSteps.end(); it++)
	{
		if ((*it)->GetRenderingStepType() != RENDERING_STEP_TYPE_1D)
		{
			this->TriggerRenderingBoundsCalculationError("incompatible rendering step type");
		}

		RenderingStep1D* currentStep = dynamic_cast<RenderingStep1D*>(*it);
		
		if (it == referencingSteps.begin())
		{
			newRenderOrigin = currentStep->GetRenderOrigin();
			newRenderSize = currentStep->GetRenderSize();
		}
		else {
			if (newRenderOrigin > currentStep->GetRenderOrigin())
			{
				newRenderSize += newRenderOrigin - currentStep->GetRenderOrigin();
				newRenderOrigin = currentStep->GetRenderOrigin();
			}

			if (newRenderSize < currentStep->GetRenderSize())
			{
				newRenderSize = currentStep->GetRenderSize();
			}
		}
	}
}