/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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
	return HeightMap::GetMemorySize(rect, renderer->GetRenderingSequence().GetRenderScale());
}

void HeightMapTransformRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->matrix.A11 << GG_STR(", ") << this->matrix.A12 << GG_STR(", ") << this->matrix.A21 << GG_STR(", ") << this->matrix.A22;
}