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

#include "HeightMapGlaciateRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds2D.hpp"
#include "../genlib/CommonProfileFactory.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapGlaciateRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* self = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());

	HeightProfile profile = CommonProfileFactory::CreateGlaciationProfile(HEIGHT_MAX, this->strength);

	if (this->includeNegative)
	{
		profile = CommonProfileFactory::CreateMirroredProfile(profile);
	}

	self->TransformHeights(&profile, Interval(0, profile.GetLength()), this->includeNegative ? HEIGHT_MIN : 1, HEIGHT_MAX);
}