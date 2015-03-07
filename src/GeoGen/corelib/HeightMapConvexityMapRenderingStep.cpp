#include "HeightMapConvexityMapRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapConvexityMapRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* other = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	HeightMap* map = new HeightMap(*other);
	map->ConvexityMap(this->radius);
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, map);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

Rectangle HeightMapConvexityMapRenderingStep::CalculateRenderingBounds(Renderer* renderer, Rectangle argumentBounds) const
{
	return Rectangle::Expand(argumentBounds, renderer->GetRenderingSequence().GetScaledSize(this->radius));
}

unsigned HeightMapConvexityMapRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return dynamic_cast<RenderingBounds2D const*>(argumentBounds[0])->GetMemorySize();
}

void HeightMapConvexityMapRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->radius;
}