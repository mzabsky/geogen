#include "HeightMapProjectionRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapProjectionRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* profile = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	HeightMap* map = new HeightMap(dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle(), 0, renderer->GetRenderingSequence().GetRenderScale());
	map->Projection(profile, this->direction);

	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, map);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

void HeightMapProjectionRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << DirectionToString(this->direction);
}

unsigned HeightMapProjectionRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetMemorySize();
}

void HeightMapProjectionRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Point thisPosition = this->GetRenderingBounds(renderer).GetPosition();
	Size2D thisSize = this->GetRenderingBounds(renderer).GetSize();

	dynamic_cast<RenderingBounds1D*>(argumentBounds[0])->CombineInterval(
		Interval(
			this->direction == DIRECTION_HORIZONTAL ? thisPosition.GetY() : thisPosition.GetX(),
			this->direction == DIRECTION_HORIZONTAL ? thisSize.GetHeight() : thisSize.GetWidth()));
}
