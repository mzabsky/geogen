#include "HeightMapProjectionRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapProjectionRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* profile = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	HeightMap* map = new HeightMap(dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle(), 0, renderer->GetRenderingSequence().GetRenderScale());
	map->Projection(profile, this->orientation);

	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, map);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

void HeightMapProjectionRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << OrientationToString(this->orientation);
}

/*void HeightMapProjectionRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
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
			RenderingBounds2D* current = dynamic_cast<RenderingBounds1D*>(*it);

			newInterval = Interval::Combine(newInterval, current->GetInterval());
		}
		else throw InternalErrorException(GG_STR("Invalid rendering step type."))
	}

	Interval calculatedInterval = this->CalculateRenderingBounds(renderer, newInterval);

	this->SetRenderingBounds(renderer, calculatedInterval);
}*/