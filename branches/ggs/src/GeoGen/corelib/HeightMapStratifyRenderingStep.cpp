#include "HeightMapStratifyRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"
#include "../genlib/CommonProfileFactory.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapStratifyRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	HeightProfile profile = CommonProfileFactory::CreateStratificationProfile(HEIGHT_MAX, this->numberOfStrata, this->steepness, this->smoothness);

	if (!this->includeNegative)
	{
		profile.ClampHeights(1, HEIGHT_MAX);
	}

	self->TransformHeights(&profile, Interval(0, profile.GetLength()), this->includeNegative ? HEIGHT_MIN : 1, HEIGHT_MAX);
}