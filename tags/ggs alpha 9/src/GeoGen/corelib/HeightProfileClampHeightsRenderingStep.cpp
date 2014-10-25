#include "HeightProfileClampHeightsRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileClampHeightsRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->ClampHeights(this->minHeight, this->maxHeight);
}

void HeightProfileClampHeightsRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->minHeight << GG_STR(", ") << this->maxHeight;
}