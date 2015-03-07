#include "HeightMapDistanceMapRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapDistanceMapRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* other = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	HeightMap* map = new HeightMap(*other);
	map->DistanceMap(this->maxDistance);
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, map);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

void HeightMapDistanceMapRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		Rectangle::Expand(this->GetRenderingBounds(renderer), renderer->GetRenderingSequence().GetScaledSize(this->maxDistance)));
}

unsigned HeightMapDistanceMapRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	RenderingBounds2D const* bounds = dynamic_cast<RenderingBounds2D const*>(argumentBounds[0]);
	return bounds->GetMemorySize() + 
		std::max(bounds->GetRectangle().GetSize().GetWidth(), bounds->GetRectangle().GetSize().GetHeight()) * (2 * sizeof(double)+sizeof(int));
}

void HeightMapDistanceMapRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->maxDistance;
}