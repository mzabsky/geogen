#include "HeightMapClampHeightsRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapClampHeightsRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	self->ClampHeights(this->minHeight, this->maxHeight);
}

void HeightMapClampHeightsRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->minHeight << GG_STR(", ") << this->maxHeight;
}