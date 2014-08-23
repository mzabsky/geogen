#include "HeightProfileCropHeightsRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileCropHeightsRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->CropHeights(this->minHeight, this->maxHeight, this->replace);
}

void HeightProfileCropHeightsRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->minHeight << GG_STR(", ") << this->maxHeight << GG_STR(", ") << this->replace;
}