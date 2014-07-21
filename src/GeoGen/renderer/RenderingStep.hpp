#pragma once

#include <vector>

#include "../CodeLocation.hpp"
#include "../Point.hpp"

namespace geogen
{
	namespace renderer
	{
		class Renderer;
		class RenderingBounds;

		enum RenderingStepType
		{
			RENDERING_STEP_TYPE_1D,
			RENDERING_STEP_TYPE_2D
		};

		class RenderingStep
		{
		private:
			CodeLocation location;
			std::vector<unsigned> argumentSlots;
			unsigned returnSlot;			

			// Non-copyable
			RenderingStep(RenderingStep const&) : location(0, 0) {};
			RenderingStep& operator=(RenderingStep const&) {};
		protected:
			void TriggerRenderingBoundsCalculationError(String message) const;			
		public:
			RenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot) : location(location), argumentSlots(argumentSlots), returnSlot(returnSlot) {}

			virtual RenderingStepType GetRenderingStepType() const = 0;
			virtual String GetName() const = 0;

			inline CodeLocation GetLocation() const { return this->location; }
			inline std::vector<unsigned> const& GetArgumentSlots() const { return this->argumentSlots; };
			inline unsigned GetReturnSlot() const { return this->returnSlot; }

			/// Executes this step.
			/// @param renderer The renderer.
			virtual void Step(Renderer* renderer) const = 0;

			/// Updates the rendering bounds based on the bounds of its arguments.
			/// @param renderer			 The renderer.
			/// @param referencingBounds The referencing bounds.
			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingBounds*> argumentBounds) const = 0;
		};
	}
}