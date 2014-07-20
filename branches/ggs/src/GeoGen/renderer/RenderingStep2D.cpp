#include "../InternalErrorException.hpp"
#include "RenderingStep2D.hpp"
#include "RenderingBounds2D.hpp"
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
	if (argumentBounds.size() == 0)
	{
		this->TriggerRenderingBoundsCalculationError("empty referencing steps list");
	}

	Rectangle newRect;
	for (std::vector<RenderingBounds*>::iterator it = argumentBounds.begin(); it != argumentBounds.end(); it++)
	{
		if ((*it)->GetRenderingStepType() != RENDERING_STEP_TYPE_2D)
		{
			this->TriggerRenderingBoundsCalculationError("incompatible rendering step type");
		}

		RenderingBounds2D* current = dynamic_cast<RenderingBounds2D*>(*it);

		if (it == argumentBounds.begin())
		{
			newRect = current->GetRectangle();
		}
		else {
			newRect = Rectangle::Combine(newRect, current->GetRectangle());
		}
	}

	this->SetRenderingBounds(renderer, newRect);
}