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

			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds);
		};
	}
}