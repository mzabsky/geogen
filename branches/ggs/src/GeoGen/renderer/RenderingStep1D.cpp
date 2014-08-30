#include "../InternalErrorException.hpp"
#include "RenderingStep1D.hpp"
#include "RenderingBounds1D.hpp"
#include "RenderingBounds2D.hpp"
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
	/*if (argumentBounds.size() == 0)
	{
		this->TriggerRenderingBoundsCalculationError("empty referencing steps list");
	}*/

	Interval thisInterval = dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetInterval();

	//Interval newInterval;
	for (std::vector<RenderingBounds*>::iterator it = argumentBounds.begin(); it != argumentBounds.end(); it++)
	{
		if ((*it)->GetRenderingStepType() == RENDERING_STEP_TYPE_1D)
		{
			RenderingBounds1D* current = dynamic_cast<RenderingBounds1D*>(*it);
			current->CombineInterval(thisInterval);

			/*
			if (it == argumentBounds.begin())
			{
				newInterval = current->GetInterval();
			}
			else 
			{*/
				//newInterval = Interval::Combine(newInterval, current->GetInterval());
			//}			
		}
		else if ((*it)->GetRenderingStepType() == RENDERING_STEP_TYPE_2D)
		{
			this->TriggerRenderingBoundsCalculationError("incorrect rendering step type");

			/*RenderingBounds2D* current = dynamic_cast<RenderingBounds2D*>(*it);

			newInterval = 
				Interval::Combine(
					newInterval, 
					Interval::Combine(
						Interval::FromRectangle(current->GetRectangle(), DIRECTION_HORIZONTAL),
						Interval::FromRectangle(current->GetRectangle(), DIRECTION_VERTICAL)));*/
		}

		else throw InternalErrorException(GG_STR("Invalid step type"));
	}

	//Interval calculatedInterval = this->CalculateRenderingBounds(renderer, newInterval);

	//this->SetRenderingBounds(renderer, calculatedInterval);
}

Interval RenderingStep1D::GetRenderingBounds(Renderer* renderer) const
{
	return dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetInterval();
}

/*Interval RenderingStep1D::CalculateRenderingBounds(Renderer* renderer, Interval argumentBounds) const
{
	return argumentBounds;
}*/