#pragma once

#include "../Size.hpp"
#include "RenderingStep.hpp"

namespace geogen
{
	namespace renderer
	{
		class Renderer;

		class RenderingStep2D : public RenderingStep
		{
		private:
			Point renderOrigin;
			Size2D renderSize;
		public:
			RenderingStep2D(CodeLocation location) : RenderingStep(location) {}

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_1D; };

			inline Point GetRenderOrigin() const { return this->renderOrigin; }
			inline Size2D GetRenderSize() const { return this->renderSize; }
		};
	}
}