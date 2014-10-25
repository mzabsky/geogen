#include "HeightMapShiftRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapShiftRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	HeightProfile* profile = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[1])->GetPtr());

	self->Shift(profile, this->maxDistance, this->direction);
}

void HeightMapShiftRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Rectangle thisRect = this->GetRenderingBounds(renderer);

	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		Rectangle::Expand(this->GetRenderingBounds(renderer), this->maxDistance, this->direction));

	dynamic_cast<RenderingBounds1D*>(argumentBounds[1])->CombineInterval(
		Interval(
		this->direction == DIRECTION_HORIZONTAL ? thisRect.GetPosition().GetY() : thisRect.GetPosition().GetX(),
		this->direction == DIRECTION_HORIZONTAL ? thisRect.GetSize().GetHeight() : thisRect.GetSize().GetWidth()));
}

void HeightMapShiftRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->maxDistance << GG_STR(", ") << DirectionToString(this->direction);
}