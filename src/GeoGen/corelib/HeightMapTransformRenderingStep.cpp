#include "HeightMapTransformRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapTransformRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	self->Transform(this->matrix, dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle());
}

void HeightMapTransformRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Rectangle thisRect = this->GetRenderingBounds(renderer);

	TransformationMatrix invertedMatrix = TransformationMatrix::Inverse(this->matrix);

	Rectangle transformedRectangle = invertedMatrix.TransformRectangle(thisRect);

	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(transformedRectangle);
}

unsigned HeightMapTransformRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	Rectangle rect = this->matrix.TransformRectangle(dynamic_cast<RenderingBounds2D const*>(argumentBounds[0])->GetRectangle());
	return HeightMap::GetMemorySize(rect);
}

void HeightMapTransformRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->matrix.A11 << GG_STR(", ") << this->matrix.A12 << GG_STR(", ") << this->matrix.A21 << GG_STR(", ") << this->matrix.A22;
}