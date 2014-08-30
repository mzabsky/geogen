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

void HeightMapMoveRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		this->GetRenderingBounds(renderer) - this->offset);
}

void HeightMapMoveRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->offset.Serialize(stream);
}
