#include "HeightMapCropHeightsRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapCropHeightsRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	self->CropHeights(this->minHeight, this->maxHeight, this->replace);
}

void HeightMapCropHeightsRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->minHeight << GG_STR(", ") << this->maxHeight << GG_STR(", ") << this->replace;
}