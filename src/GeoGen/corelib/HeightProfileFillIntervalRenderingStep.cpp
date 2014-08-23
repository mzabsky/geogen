#include "HeightProfileFillIntervalRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileFillIntervalRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->FillInterval(this->interval, this->height);
}

void HeightProfileFillIntervalRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->interval.Serialize(stream);
	stream << GG_STR(", ") << this->height;
}