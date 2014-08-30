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
			this->TriggerRenderingBoundsCalculationError("incorrect rendering step type");
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