#include "HeightMapRadialGradientRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapRadialGradientRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* map = new HeightMap(dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle(), 0, renderer->GetRenderingSequence().GetRenderScale());
	map->RadialGradient(this->point, this->radius, this->fromHeight, this->toHeight);
	
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, map);
	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

void HeightMapRadialGradientRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->point.Serialize(stream);

	stream << GG_STR(", ") << this->radius << GG_STR(", ") << this->fromHeight << GG_STR(", ") << this->toHeight;
}