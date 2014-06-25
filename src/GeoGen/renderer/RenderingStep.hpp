#pragma once

#include <vector>

#include "../CodeLocation.hpp"
#include "../Point.hpp"

namespace geogen
{
	namespace renderer
	{
		class Renderer;

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
			inline const std::vector<unsigned> GetArgumentSlots() const { return this->argumentSlots; };
			inline unsigned GetReturnSlot() const { return this->returnSlot; }

			virtual void Step(Renderer* renderer) const = 0;
			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingStep*> referencingSteps) = 0;
		};
	}
}