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

void HeightMapRescaleRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Rectangle thisRect = this->GetRenderingBounds(renderer);

	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		Rectangle(
			Point(Height(thisRect.GetPosition().GetX() / this->horizontalScale), Height(thisRect.GetPosition().GetY() / this->verticalScale)),
			Size2D(RoundAway(thisRect.GetSize().GetWidth() / this->horizontalScale), RoundAway(thisRect.GetSize().GetHeight() / this->verticalScale))));
}

void HeightMapRescaleRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->horizontalScale << GG_STR(", ") << this->horizontalScale;
}