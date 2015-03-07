#include "HeightMapFlatRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapFlatRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* profile = new HeightMap(dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle(), this->height, renderer->GetRenderingSequence().GetRenderScale());
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, profile);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

unsigned HeightMapFlatRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetMemorySize();
}

void HeightMapFlatRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->height;
}