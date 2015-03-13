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

#include "HeightMapDistortRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapDistortRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	HeightMap* horizontalDistortionMap = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[1])->GetPtr());
	HeightMap* verticalDistortionMap = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[2])->GetPtr());

	self->Distort(horizontalDistortionMap, verticalDistortionMap, this->maxDistance);
}

void HeightMapDistortRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		Rectangle::Expand(this->GetRenderingBounds(renderer), renderer->GetRenderingSequence().GetScaledSize(this->maxDistance)));

	dynamic_cast<RenderingBounds2D*>(argumentBounds[1])->CombineRectangle(this->GetRenderingBounds(renderer));
	dynamic_cast<RenderingBounds2D*>(argumentBounds[2])->CombineRectangle(this->GetRenderingBounds(renderer));
}

unsigned HeightMapDistortRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return 2 * dynamic_cast<RenderingBounds2D const*>(argumentBounds[0])->GetMemorySize(renderer->GetRenderingSequence().GetRenderScale());
}

void HeightMapDistortRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->maxDistance;
}