#include "HeightProfileSliceRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileSliceRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* profile = new HeightProfile(dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetInterval(), 0, renderer->GetRenderingSequence().GetRenderScale());
	HeightMap* heightMap = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	profile->Slice(heightMap, this->direction, this->coordinate);
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_PROFILE, profile);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

/*void RenderingStep1D::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{

	Interval newInterval;
	for (std::vector<RenderingBounds*>::iterator it = argumentBounds.begin(); it != argumentBounds.end(); it++)
	{
		if ((*it)->GetRenderingStepType() == RENDERING_STEP_TYPE_1D)
		{
			RenderingBounds1D* current = dynamic_cast<RenderingBounds1D*>(*it);

			newInterval = Interval::Combine(newInterval, current->GetInterval());
		}
		else if ((*it)->GetRenderingStepType() == RENDERING_STEP_TYPE_2D)
		{
			RenderingBounds2D* current = dynamic_cast<RenderingBounds2D*>(*it);

			newInterval =
				Interval::Combine(
				newInterval,
				Interval::Combine(
				Interval::FromRectangle(current->GetRectangle(), DIRECTION_HORIZONTAL),
				Interval::FromRectangle(current->GetRectangle(), DIRECTION_VERTICAL)));
		}

		else throw InternalErrorException(GG_STR("Invalid step type"));
	}

	Interval calculatedInterval = this->CalculateRenderingBounds(renderer, newInterval);

	this->SetRenderingBounds(renderer, calculatedInterval);
}*/

void HeightProfileSliceRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << DirectionToString(this->direction) << GG_STR(", ") << this->coordinate;
}