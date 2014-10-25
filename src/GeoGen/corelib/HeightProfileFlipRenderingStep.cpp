#include "HeightProfileFlipRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileFlipRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->Flip();
}

void HeightProfileFlipRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Interval current = this->GetRenderingBounds(renderer);
	Interval flippedInterval = Interval(-current.GetStart() - current.GetLength(), current.GetLength());

	dynamic_cast<RenderingBounds1D*>(argumentBounds[0])->CombineInterval(flippedInterval);
}