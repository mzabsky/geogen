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
			Size2D((Size1D)RoundAway(thisRect.GetSize().GetWidth() / this->horizontalScale), (Size1D)RoundAway(thisRect.GetSize().GetHeight() / this->verticalScale))));
}

unsigned HeightMapRescaleRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	Rectangle thisRect = dynamic_cast<RenderingBounds2D const*>(argumentBounds[0])->GetRectangle();
	Rectangle newRectangle(Point(Coordinate(thisRect.GetPosition().GetX() * horizontalScale), Coordinate(thisRect.GetPosition().GetY() * verticalScale)), Size2D(Size1D(thisRect.GetSize().GetWidth() * horizontalScale), Size1D(thisRect.GetSize().GetHeight() * verticalScale)));

	return HeightMap::GetMemorySize(newRectangle, renderer->GetRenderingSequence().GetRenderScale());
}

void HeightMapRescaleRenderingStep::SimulateOnRenderingBounds(RenderingBounds* renderingBounds) const
{
	RenderingBounds2D* bounds2d = dynamic_cast<RenderingBounds2D*>(renderingBounds);
	Rectangle newRectangle(Point(Coordinate(bounds2d->GetRectangle().GetPosition().GetX() * this->horizontalScale), Coordinate(bounds2d->GetRectangle().GetPosition().GetY() * this->verticalScale)), Size2D(Size1D(bounds2d->GetRectangle().GetSize().GetWidth() * this->horizontalScale), Size1D(bounds2d->GetRectangle().GetSize().GetHeight() * this->verticalScale)));

	bounds2d->SetRectangle(newRectangle);
}

void HeightMapRescaleRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->horizontalScale << GG_STR(", ") << this->verticalScale;
}