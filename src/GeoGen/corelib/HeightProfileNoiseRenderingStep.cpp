#include "HeightProfileNoiseRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileNoiseRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->NoiseLayer(this->waveLength, this->amplitude, this->seed, this->seedStep);
}

void HeightProfileNoiseRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << waveLength << GG_STR(", ") << amplitude << GG_STR(", ") << this->seed << GG_STR(", ") << this->seedStep;
}