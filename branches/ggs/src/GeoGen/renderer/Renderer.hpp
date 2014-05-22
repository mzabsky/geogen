#pragma once

#include <vector>

#include "../Point.hpp"
#include "RenderingSequence.hpp"

namespace geogen
{
	namespace renderer
	{
		class RenderingStep;
		class RendererObject;		

		class Renderer
		{
		private:
			std::vector<unsigned> referencedSlots;
			unsigned affectedSlot;
			Point renderOrigin;
			Point renderSize;
			RenderingSequence& renderingSequence;
		public:
			Renderer(RenderingSequence& renderingSequence, unsigned affectedSlot, std::vector<unsigned> referencedSlots)
				: renderingSequence(renderingSequence), affectedSlot(affectedSlot), referencedSlots(referencedSlots) {}

			inline const std::vector<unsigned> GetReferencedSlots() const { return this->referencedSlots; };
			inline unsigned GetAffectedSlot() const { return this->affectedSlot; }

			inline Point GetRenderOrigin() const { return this->renderOrigin; }
			inline Point GetRenderSize() const { return this->renderSize; }

			virtual void Step(Renderer* renderer) const = 0;
			virtual void UpdateRenderingBounds(Renderer* renderer, std::vector<RenderingStep*> referencingSteps) const = 0;
		};
	}
}