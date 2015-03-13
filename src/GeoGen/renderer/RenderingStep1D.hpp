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

#include "../Interval.hpp"
#include "RenderingStep.hpp"

namespace geogen
{
	namespace renderer
	{
		class Renderer;
		class RenderingBounds;

		class RenderingStep1D : public RenderingStep
		{
		private:
		protected:
			void SetRenderingBounds(Renderer* renderer, Interval interval) const;
		public:
			RenderingStep1D(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot) : RenderingStep(location, argumentSlots, returnSlot)
			{}

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_1D; };

			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const;
			Interval GetRenderingBounds(Renderer* renderer) const;
		};
	}
}