#include "HeightMapCropRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapCropRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	self->Crop(this->rectangle, this->height);
	self->Resize(this->GetRenderingBounds(renderer), this->height);
}

void HeightMapCropRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		Rectangle::Intersect(this->GetRenderingBounds(renderer), this->rectangle));
}

void HeightMapCropRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->rectangle.Serialize(stream);

	stream << GG_STR(", ") << this->height;
}