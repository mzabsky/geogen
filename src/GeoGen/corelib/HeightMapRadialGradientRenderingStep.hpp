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

#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapRadialGradientRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Point point;
			Height radius;
			Height fromHeight;
			Height toHeight;
		public:
			HeightMapRadialGradientRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Point point, Size1D radius, Height fromHeight, Height toHeight)
				: RenderingStep2D(location, argumentSlots, returnSlot), point(point), radius(radius), fromHeight(fromHeight), toHeight(toHeight) {};

			virtual String GetName() const { return GG_STR("HeightMap.RadialGradient"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual unsigned GetPeakExtraMemory(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds const*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}