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

#include "../Direction.hpp"
#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapShiftRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Size1D maxDistance;
			Direction direction;
		public:
			HeightMapShiftRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Size1D maxDistance, Direction direction)
				: RenderingStep2D(location, argumentSlots, returnSlot), maxDistance(maxDistance), direction(direction) {};

			virtual String GetName() const { return GG_STR("HeightMap.Shift"); };

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}