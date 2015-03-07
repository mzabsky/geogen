#include "HeightMapPatternRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapPatternRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* map = new HeightMap(dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle(), 0, renderer->GetRenderingSequence().GetRenderScale());
	HeightMap* pattern = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	map->Pattern(pattern, this->repeatRectangle);

	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, map);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

void HeightMapPatternRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		this->repeatRectangle);
}

unsigned HeightMapPatternRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetMemorySize();
}

void HeightMapPatternRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->repeatRectangle.Serialize(stream);
}