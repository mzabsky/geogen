#include "HeightMapGradientRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapGradientRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* map = new HeightMap(dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle(), 0, renderer->GetRenderingSequence().GetRenderScale());
	map->Gradient(this->source, this->destination, this->fromHeight, this->toHeight);

	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, map);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

void HeightMapGradientRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->source.Serialize(stream);
	stream << GG_STR(", ");
	this->destination.Serialize(stream);
	stream << GG_STR(", ") << fromHeight << GG_STR(", ") << toHeight;
}