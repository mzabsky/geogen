#include "HeightProfileCombineRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileCombineRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	HeightProfile* addend = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[1])->GetPtr());
	HeightProfile* mask = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[2])->GetPtr());
	self->Combine(addend, mask);
}