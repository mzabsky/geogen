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

#include "../Rectangle.hpp"
#include "RenderingStep.hpp"
#include "RenderingBounds.hpp"
#include "../genlib/HeightMap.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingBounds2D : public RenderingBounds
		{
		private:
			Rectangle rectangle;
		public:
			RenderingBounds2D(Rectangle rectangle) : rectangle(rectangle) {}

			inline Rectangle GetRectangle() const { return this->rectangle; }
			inline void SetRectangle(Rectangle rectangle) { this->rectangle = rectangle; }
			inline void CombineRectangle(Rectangle rectangle) { this->rectangle = Rectangle::Combine(rectangle, this->rectangle); }

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_2D; };

			virtual unsigned GetMemorySize(Scale scale) const { return genlib::HeightMap::GetMemorySize(this->rectangle, scale); };

			virtual void Serialize(IOStream& stream) const
			{
				this->rectangle.Serialize(stream);
			}
		};
	}
}