#pragma once

#include "RenderingStep.hpp"

namespace geogen
{
	namespace renderer
	{
		/// Contains rendering bounds for a RenderingStep.
		class RenderingBounds
		{
		private:
		public:

			/// Gets the type of the rendering step to which this object is associated. Indicates the specific derived type of this instance.
			/// @return The rendering step type.
			virtual RenderingStepType GetRenderingStepType() const = 0;
		};
	}
}