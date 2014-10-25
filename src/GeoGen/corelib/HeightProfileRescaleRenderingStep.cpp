#include "HeightProfileRescaleRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileRescaleRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->Rescale(this->scale);
}

void HeightProfileRescaleRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Interval thisInterval = this->GetRenderingBounds(renderer);

	dynamic_cast<RenderingBounds1D*>(argumentBounds[0])->CombineInterval(
		Interval(
		(Coordinate)RoundAway(thisInterval.GetStart() / this->scale),
		(Size1D)RoundAway(thisInterval.GetLength() / this->scale)));
}

void HeightProfileRescaleRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->scale;
}