#include "HeightMapDistortRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapDistortRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	HeightMap* horizontalDistortionMap = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[1])->GetPtr());
	HeightMap* verticalDistortionMap = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[2])->GetPtr());

	self->Distort(horizontalDistortionMap, verticalDistortionMap, this->maxDistance);
}

void HeightMapDistortRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		Rectangle::Expand(this->GetRenderingBounds(renderer), renderer->GetRenderingSequence().GetScaledSize(this->maxDistance)));

	dynamic_cast<RenderingBounds2D*>(argumentBounds[1])->CombineRectangle(this->GetRenderingBounds(renderer));
	dynamic_cast<RenderingBounds2D*>(argumentBounds[2])->CombineRectangle(this->GetRenderingBounds(renderer));
}

unsigned HeightMapDistortRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return 2 * dynamic_cast<RenderingBounds2D const*>(argumentBounds[0])->GetMemorySize(renderer->GetRenderingSequence().GetRenderScale());
}

void HeightMapDistortRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->maxDistance;
}