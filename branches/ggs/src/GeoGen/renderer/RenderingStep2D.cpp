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
	/*if (argumentBounds.size() == 0)
	{
		this->TriggerRenderingBoundsCalculationError("empty referencing steps list");
	}*/

	Rectangle newRect;
	for (std::vector<RenderingBounds*>::iterator it = argumentBounds.begin(); it != argumentBounds.end(); it++)
	{
		if ((*it)->GetRenderingStepType() == RENDERING_STEP_TYPE_1D)
		{
			RenderingBounds1D* current = dynamic_cast<RenderingBounds1D*>(*it);

			//this->TriggerRenderingBoundsCalculationError("incompatible rendering step type");
			newRect = Rectangle::Combine(newRect, Rectangle(Point(current->GetInterval().GetStart(), current->GetInterval().GetStart()), Size2D(current->GetInterval().GetLength(), current->GetInterval().GetLength())));
		}
		else if ((*it)->GetRenderingStepType() == RENDERING_STEP_TYPE_2D)
		{
			RenderingBounds2D* current = dynamic_cast<RenderingBounds2D*>(*it);

			if (it == argumentBounds.begin())
			{
				newRect = current->GetRectangle();
			}
			else
			{
				newRect = Rectangle::Combine(newRect, current->GetRectangle());
			}
		}
		else throw InternalErrorException(GG_STR("Invalid rendering step type."));
	}

	Rectangle calculatedRect = this->CalculateRenderingBounds(renderer, newRect);

	this->SetRenderingBounds(renderer, calculatedRect);
}

Rectangle RenderingStep2D::CalculateRenderingBounds(Renderer* renderer, Rectangle argumentBounds) const
{
	return argumentBounds;
}