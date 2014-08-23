#include "HeightProfileBlurRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileBlurRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->Blur(this->radius);
}

void HeightProfileBlurRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->radius;
}

Interval HeightProfileBlurRenderingStep::CalculateRenderingBounds(Renderer* renderer, Interval argumentBounds) const
{
	return Interval::Expand(argumentBounds, this->radius);
}