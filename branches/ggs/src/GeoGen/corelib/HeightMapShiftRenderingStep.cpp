#include "HeightMapShiftRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"

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

Rectangle HeightMapShiftRenderingStep::CalculateRenderingBounds(Renderer* renderer, Rectangle argumentBounds) const
{
	if (this->direction == DIRECTION_HORIZONTAL)
	{
		return Rectangle(argumentBounds.GetPosition() - Point(this->maxDistance, 0), argumentBounds.GetSize() + Size2D(this->maxDistance * 2, 0));
	}
	else if (this->direction == DIRECTION_VERTICAL)
	{
		return Rectangle(argumentBounds.GetPosition() - Point(0, this->maxDistance), argumentBounds.GetSize() + Size2D(0, this->maxDistance * 2));
	}
}

void HeightMapShiftRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->maxDistance << GG_STR(", ") << DirectionToString(this->direction);
}