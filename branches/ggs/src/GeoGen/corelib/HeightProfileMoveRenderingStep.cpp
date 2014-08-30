#include "HeightProfileMoveRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileMoveRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->Move(this->offset);
}

void HeightProfileMoveRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds1D*>(argumentBounds[0])->CombineInterval(
		this->GetRenderingBounds(renderer) - this->offset);
}

void HeightProfileMoveRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->offset;
}