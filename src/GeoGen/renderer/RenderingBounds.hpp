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
#include "RenderingStep.hpp"

namespace geogen
{
	namespace renderer
	{
		/// Contains rendering bounds for a RenderingStep.
		class RenderingBounds: public Serializable
		{
		private:
		public:
			/// Gets the type of the rendering step to which this object is associated. Indicates the specific derived type of this instance.
			/// @return The rendering step type.
			virtual RenderingStepType GetRenderingStepType() const = 0;

			/// Gets the number of bytes that will be required to allocate object of size specified by these
			/// bounds.
			/// @param scale The scale.
			/// @return The memory size, in bytes.
			virtual unsigned GetMemorySize(Scale scale) const = 0;

			virtual void Serialize(IOStream& stream) const = 0;
		};
	}
}