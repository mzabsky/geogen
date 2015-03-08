#include "HeightMapAddRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapAddRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	
	if (this->GetArgumentSlots().size() == 1)
	{
		// No mask
		self->Add(this->addend);
	}
	else
	{
		// Has mask
		HeightMap* mask = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[1])->GetPtr());
		self->AddMasked(this->addend, mask);
	}
}

void HeightMapAddRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->addend;
}