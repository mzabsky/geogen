#pragma once

#include "../Size.hpp"
#include "RenderingStep.hpp"

namespace geogen
{
	namespace renderer
	{
		class Renderer;

		class RenderingStep1D : public RenderingStep
		{
		private:
			Coordinate renderOrigin;
			Size1D renderSize;			
		public:
			RenderingStep1D(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot) : RenderingStep(location, argumentSlots, returnSlot)
			{}

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_1D; };

			inline Coordinate GetRenderOrigin() const { return this->renderOrigin; }
			inline Size1D GetRenderSize() const { return this->renderSize; }

			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingStep*> referencingSteps);
		};
	}
}