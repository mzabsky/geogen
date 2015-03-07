#include "HeightProfileSliceRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds1D.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileSliceRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* profile = new HeightProfile(dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetInterval(), 0, renderer->GetRenderingSequence().GetRenderScale());
	HeightMap* heightMap = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	profile->Slice(heightMap, this->direction, this->coordinate);
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_PROFILE, profile);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

void HeightProfileSliceRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Interval thisInterval = this->GetRenderingBounds(renderer);

	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		this->direction == DIRECTION_HORIZONTAL ?
		Rectangle(Point(thisInterval.GetStart(), this->coordinate), Size2D(thisInterval.GetLength(), 1)) :
		Rectangle(Point(this->coordinate, thisInterval.GetStart()), Size2D(1, thisInterval.GetLength())));
}

unsigned HeightProfileSliceRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetMemorySize();
}

void HeightProfileSliceRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << DirectionToString(this->direction) << GG_STR(", ") << this->coordinate;
}