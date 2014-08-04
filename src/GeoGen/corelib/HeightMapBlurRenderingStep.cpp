#include "HeightMapBlurRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapBlurRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	self->Blur(this->radius);
}

Rectangle HeightMapBlurRenderingStep::CalculateRenderingBounds(Renderer* renderer, Rectangle argumentBounds) const
{
	return Rectangle::Expand(argumentBounds, this->radius);
}

void HeightMapBlurRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->radius;
}