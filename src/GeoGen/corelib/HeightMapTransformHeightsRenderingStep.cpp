#include "HeightMapTransformHeightsRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapTransformHeightsRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	HeightProfile* profile = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[1])->GetPtr());

	self->TransformHeights(profile, this->interval, this->minHeight, this->maxHeight);
}

void HeightMapTransformHeightsRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Rectangle thisRect = this->GetRenderingBounds(renderer);

	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(this->GetRenderingBounds(renderer));

	dynamic_cast<RenderingBounds1D*>(argumentBounds[1])->CombineInterval(this->interval);
}

void HeightMapTransformHeightsRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->interval.Serialize(stream);
	stream << GG_STR(this->minHeight) << GG_STR(this->maxHeight);
}