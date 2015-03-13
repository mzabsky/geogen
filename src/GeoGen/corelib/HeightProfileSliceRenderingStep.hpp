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
#include "../Direction.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileSliceRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Direction direction;
			Coordinate coordinate;
			Height heightFrom, heightTo;
		public:
			HeightProfileSliceRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Direction direction, Coordinate coordinate)
				: RenderingStep1D(location, argumentSlots, returnSlot), direction(direction), coordinate(coordinate) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Slice"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual unsigned GetPeakExtraMemory(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds const*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}