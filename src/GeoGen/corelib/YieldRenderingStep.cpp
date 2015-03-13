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

#include "../InternalErrorException.hpp"
#include "YieldRenderingStep.hpp"
#include "../renderer/RenderingBounds2D.hpp"
#include "../renderer/Renderer.hpp"
#include "../Rectangle.hpp"
#include "../renderer/RendererObject.hpp"
#include "../genlib/HeightMap.hpp"

using namespace std;
using namespace geogen;
using namespace corelib;
using namespace renderer;
using namespace genlib;

void YieldRenderingStep::Step(renderer::Renderer* renderer) const
{
	HeightMap* internalData = dynamic_cast<HeightMap*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	HeightMap* copiedData = new HeightMap(*internalData, this->rect);

	if (!renderer->GetRenderedMapTable().AddItem(this->name, copiedData))
	{
		delete copiedData;
	}
}

void YieldRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->CombineRectangle(this->rect);

	dynamic_cast<RenderingBounds2D*>(argumentBounds[0])->CombineRectangle(
		this->GetRenderingBounds(renderer));
}

void YieldRenderingStep::SerializeArguments(IOStream& stream) const
{
	this->rect.Serialize(stream);

	stream << GG_STR(", ") << this->name;
}