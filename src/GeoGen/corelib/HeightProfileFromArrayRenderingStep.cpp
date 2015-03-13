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

#include "HeightProfileFromArrayRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;
using namespace std;

void HeightProfileFromArrayRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* profile = new HeightProfile(dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetInterval(), 0, renderer->GetRenderingSequence().GetRenderScale());
	profile->FromArray(this->heights);
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_PROFILE, profile);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}

unsigned HeightProfileFromArrayRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	return dynamic_cast<RenderingBounds1D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetMemorySize(renderer->GetRenderingSequence().GetRenderScale());
}

void HeightProfileFromArrayRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << GG_STR("{");
	for (map<Coordinate, Height>::const_iterator it = this->heights.begin(); it != this->heights.end(); it++)
	{
		stream << GG_STR("[") << it->first << GG_STR(", ") << it->second << GG_STR("], ");
	}
	stream << GG_STR("}");
}