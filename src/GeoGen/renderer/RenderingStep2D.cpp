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
#include "RenderingStep2D.hpp"
#include "RenderingBounds2D.hpp"
#include "RenderingBounds1D.hpp"
#include "Renderer.hpp"
#include "../Interval.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;

void RenderingStep2D::SetRenderingBounds(Renderer* renderer, Rectangle rectangle) const
{
	dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->SetRectangle(rectangle);
}

void RenderingStep2D::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Rectangle thisRectangle = dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle();

	for (std::vector<RenderingBounds*>::iterator it = argumentBounds.begin(); it != argumentBounds.end(); it++)
	{
		if ((*it)->GetRenderingStepType() == RENDERING_STEP_TYPE_1D)
		{
			this->TriggerRenderingBoundsCalculationError(GG_STR("Incorrect rendering step type"));
		}
		else if ((*it)->GetRenderingStepType() == RENDERING_STEP_TYPE_2D)
		{
			RenderingBounds2D* current = dynamic_cast<RenderingBounds2D*>(*it);
			current->CombineRectangle(thisRectangle);
		}

		else throw InternalErrorException(GG_STR("Invalid step type"));
	}
}

Rectangle RenderingStep2D::GetRenderingBounds(Renderer* renderer) const
{
	return dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle();
}

/*Rectangle RenderingStep2D::CalculateRenderingBounds(Renderer* renderer, Rectangle argumentBounds) const
{
	return argumentBounds;
}*/