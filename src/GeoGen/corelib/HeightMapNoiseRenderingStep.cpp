#include "HeightMapNoiseRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapNoiseRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->NoiseLayer(this->waveLength, this->amplitude, this->seed, this->seedStep);
}

void HeightMapNoiseRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << waveLength << GG_STR(", ") << amplitude << GG_STR(", ") << this->seed << GG_STR(", ") << this->seedStep;
}