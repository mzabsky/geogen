#pragma once

#include "RenderingStep.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingBounds
		{
		private:
		public:
			virtual RenderingStepType GetRenderingStepType() const = 0;
		};
	}
}