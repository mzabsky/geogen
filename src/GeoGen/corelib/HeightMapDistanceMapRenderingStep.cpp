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

#include "HeightMapDistanceMapRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace std;
using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapDistanceMapRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* other = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	HeightMap* map = new HeightMap(*other);
	map->DistanceMap(this->maxDistance);
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, map);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

void HeightMapDistanceMapRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		Rectangle::Expand(this->GetRenderingBounds(renderer), renderer->GetRenderingSequence().GetScaledSize(this->maxDistance)));
}

unsigned HeightMapDistanceMapRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	Scale scale = renderer->GetRenderingSequence().GetRenderScale();
	RenderingBounds2D const* bounds = dynamic_cast<RenderingBounds2D const*>(argumentBounds[0]);
	return bounds->GetMemorySize(scale) +
		std::max(
			(bounds->GetRectangle() * scale).GetSize().GetWidth(), 
			(bounds->GetRectangle() * scale).GetSize().GetHeight()
		) * (2 * sizeof(double)+sizeof(int));
}

void HeightMapDistanceMapRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->maxDistance;
}