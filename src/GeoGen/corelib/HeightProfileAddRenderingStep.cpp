#include "HeightProfileAddRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileAddRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	if (this->GetArgumentSlots().size() == 1)
	{
		// No mask
		self->Add(this->addend);
	}
	else
	{
		// Has mask
		HeightProfile* mask = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[1])->GetPtr());
		self->AddMasked(this->addend, mask);
	}
}

void HeightProfileAddRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->addend;
}