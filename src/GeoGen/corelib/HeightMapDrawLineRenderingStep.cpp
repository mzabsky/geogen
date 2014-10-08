#include "HeightMapDrawLineRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapDrawLineRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	self->DrawLine(this->start, this->end, this->height);
}

void HeightMapDrawLineRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->start.Serialize(stream);
	stream << GG_STR(", ");
	this->end.Serialize(stream);
	stream << GG_STR(", ") << this->height;
	
}