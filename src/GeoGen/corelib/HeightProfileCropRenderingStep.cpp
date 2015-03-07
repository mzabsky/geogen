#include "HeightProfileCropRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileCropRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->Crop(this->interval, this->height);
	self->Resize(this->GetRenderingBounds(renderer), height);
}

void HeightProfileCropRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds1D*>(argumentBounds[0])->CombineInterval(
		Interval::Intersect(this->GetRenderingBounds(renderer), this->interval));
}

unsigned HeightProfileCropRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetMemorySize();
}

void HeightProfileCropRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->interval.Serialize(stream);
	stream << GG_STR(", ") << this->height;
}