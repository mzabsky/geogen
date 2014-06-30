#pragma once

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
		public:
			RenderingStep1D(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot) : RenderingStep(location, argumentSlots, returnSlot)
			{}

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_1D; };

			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds);
		};
	}
}