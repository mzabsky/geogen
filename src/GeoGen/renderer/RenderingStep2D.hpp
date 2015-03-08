#pragma once

#include "../Rectangle.hpp"
#include "RenderingStep.hpp"

namespace geogen
{
	namespace renderer
	{
		class Renderer;
		class RenderingBounds;

		class RenderingStep2D : public RenderingStep
		{
		protected:
			void SetRenderingBounds(Renderer* renderer, Rectangle rectangle) const;
		public:
			RenderingStep2D(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot) : RenderingStep(location, argumentSlots, returnSlot)
			{}

			virtual RenderingStepType GetRenderingStepType() const { return RENDERING_STEP_TYPE_2D; };			

			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const;

			Rectangle GetRenderingBounds(Renderer* renderer) const;
			//virtual Rectangle CalculateRenderingBounds(Renderer* renderer, Rectangle argumentBounds) const;
		};
	}
}