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
			this->TriggerRenderingBoundsCalculationError(GG_STR("incorrect rendering step type"));

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