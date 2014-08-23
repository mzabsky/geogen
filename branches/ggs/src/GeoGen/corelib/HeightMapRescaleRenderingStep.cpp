#include "HeightMapRescaleRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapRescaleRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	self->Rescale(this->horizontalScale, this->verticalScale);
}

Rectangle HeightMapRescaleRenderingStep::CalculateRenderingBounds(Renderer* renderer, Rectangle argumentBounds) const
{	 
	return Rectangle(Point(RoundAway(argumentBounds.GetPosition().GetX() / this->horizontalScale), RoundAway(argumentBounds.GetPosition().GetY() / this->verticalScale)), Size2D(argumentBounds.GetSize().GetWidth() / this->horizontalScale, argumentBounds.GetSize().GetHeight() / this->verticalScale));
}

void HeightMapRescaleRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->horizontalScale << GG_STR(", ") << this->horizontalScale;
}