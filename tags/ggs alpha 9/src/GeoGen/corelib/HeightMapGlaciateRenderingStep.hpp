#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapGlaciateRenderingStep : public renderer::RenderingStep2D
		{
		private:
			double strength;
			bool includeNegative;
		public:
			HeightMapGlaciateRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, double strength, bool includeNegative)
				: RenderingStep2D(location, argumentSlots, returnSlot), strength(strength), includeNegative(includeNegative) {};

			virtual String GetName() const { return GG_STR("HeightMap.Glaciate"); };

			virtual void Step(renderer::Renderer* renderer) const;
		};
	}
}