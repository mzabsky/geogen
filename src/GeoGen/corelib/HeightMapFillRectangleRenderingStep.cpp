#include "HeightMapFillRectangleRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapFillRectangleRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	
	self->FillRectangle(this->fillRectangle, this->height);
}

void HeightMapFillRectangleRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->fillRectangle.Serialize(stream);

	stream << GG_STR(", ") << this->height;
}