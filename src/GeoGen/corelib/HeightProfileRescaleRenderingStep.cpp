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

#include "HeightProfileRescaleRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightProfile.hpp"
#include "../renderer/RenderingBounds1D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightProfileRescaleRenderingStep::Step(Renderer* renderer) const
{
	HeightProfile* self = dynamic_cast<HeightProfile*>(renderer->GetObjectTable().GetObject(this->GetArgumentSlots()[0])->GetPtr());
	self->Rescale(this->scale);
}

void HeightProfileRescaleRenderingStep::UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const
{
	Interval thisInterval = this->GetRenderingBounds(renderer);

	dynamic_cast<RenderingBounds1D*>(argumentBounds[0])->CombineInterval(
		Interval(
		(Coordinate)RoundAway(thisInterval.GetStart() / this->scale),
		(Size1D)RoundAway(thisInterval.GetLength() / this->scale)));
}

unsigned HeightProfileRescaleRenderingStep::GetPeakExtraMemory(Renderer* renderer, std::vector<RenderingBounds const*> argumentBounds) const
{
	Interval thisInterval = dynamic_cast<RenderingBounds1D const*>(argumentBounds[0])->GetInterval();
	Interval newInterval(Coordinate(thisInterval.GetStart() * scale), Size1D(thisInterval.GetLength() * scale));

	return HeightProfile::GetMemorySize(newInterval, renderer->GetRenderingSequence().GetRenderScale());
}

void HeightProfileRescaleRenderingStep::SimulateOnRenderingBounds(RenderingBounds* renderingBounds) const
{
	RenderingBounds1D* bounds1d = dynamic_cast<RenderingBounds1D*>(renderingBounds);
	Interval newInterval(Coordinate(bounds1d->GetInterval().GetStart() * scale), Size1D(bounds1d->GetInterval().GetLength() * scale));

	bounds1d->SetInterval(newInterval);
}

void HeightProfileRescaleRenderingStep::SerializeArguments(IOStream& stream) const
{
	stream << this->scale;
}