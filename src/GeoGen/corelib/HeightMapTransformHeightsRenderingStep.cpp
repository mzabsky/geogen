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

#include "HeightMapTransformHeightsRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapTransformHeightsRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	HeightProfile* profile = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[1])->GetPtr());

	self->TransformHeights(profile, this->interval, this->minHeight, this->maxHeight);
}

void HeightMapTransformHeightsRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Rectangle thisRect = this->GetRenderingBounds(renderer);

	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(this->GetRenderingBounds(renderer));

	dynamic_cast<RenderingBounds1D*>(argumentBounds[1])->CombineInterval(this->interval);
}

void HeightMapTransformHeightsRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->interval.Serialize(stream);
	stream << this->minHeight << this->maxHeight;
}