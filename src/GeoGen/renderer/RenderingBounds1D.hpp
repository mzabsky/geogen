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
#include "../Interval.hpp"
#include "RenderingStep.hpp"
#include "RenderingBounds.hpp"
#include "../genlib/HeightProfile.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingBounds1D : public RenderingBounds
		{
		private:
			Interval interval;
		public:
			RenderingBounds1D(Interval interval) : interval(interval) {}

			inline Interval GetInterval() const { return this->interval; }
			inline void SetInterval(Interval interval) { this->interval = interval; }
			inline void CombineInterval(Interval interval) { this->interval = Interval::Combine(interval, this->interval); }

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_1D; };

			virtual unsigned GetMemorySize(Scale scale) const { return genlib::HeightProfile::GetMemorySize(this->interval, scale); };

			virtual void Serialize(IOStream& stream) const
			{
				this->interval.Serialize(stream);
			}
		};
	}
}