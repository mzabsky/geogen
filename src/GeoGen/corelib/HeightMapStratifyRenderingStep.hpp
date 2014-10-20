#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapStratifyRenderingStep : public renderer::RenderingStep2D
		{
		private:
			unsigned numberOfStrata;
			double steepness;
			double smoothness;
			bool includeNegative;
		public:
			HeightMapStratifyRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, unsigned numberOfStrata, double steepness, double smoothness, bool includeNegative)
				: RenderingStep2D(location, argumentSlots, returnSlot), numberOfStrata(numberOfStrata), steepness(steepness), smoothness(smoothness), includeNegative(includeNegative) {};

			virtual String GetName() const { return GG_STR("HeightMap.Stratify"); };

			virtual void Step(renderer::Renderer* renderer) const;
		};
	}
}