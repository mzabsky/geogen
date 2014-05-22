#pragma once

#include <vector>

#include "../CodeLocation.hpp"
#include "../Point.hpp"

namespace geogen
{
	namespace renderer
	{
		class Renderer;

		class RenderingStep
		{
		private:
			CodeLocation location;
			std::vector<unsigned> referencedSlots;
			unsigned affectedSlot;
			Point renderOrigin;
			Point renderSize;
		public:
			RenderingStep(CodeLocation location) : location(location) {}

			inline const std::vector<unsigned> GetReferencedSlots() const { return this->referencedSlots; };
			inline unsigned GetAffectedSlot() const { return this->affectedSlot; }

			inline Point GetRenderOrigin() const { return this->renderOrigin; }
			inline Point GetRenderSize() const { return this->renderSize; }

			virtual void Step(Renderer* renderer) const = 0;
			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingStep*> referencingSteps) const = 0;
		};
	}
}