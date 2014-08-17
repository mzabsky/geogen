#include "HeightMapMoveRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapMoveRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->Move(this->offset);
}

Rectangle HeightMapMoveRenderingStep::CalculateRenderingBounds(Renderer* renderer, Rectangle argumentBounds) const
{
	return argumentBounds - this->offset;
}

void HeightMapMoveRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->offset.Serialize(stream);
}
