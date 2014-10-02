#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileFlipRenderingStep : public renderer::RenderingStep1D
		{
		private:
		public:
			HeightProfileFlipRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot)
				: RenderingStep1D(location, argumentSlots, returnSlot) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Flip"); };

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual void Step(renderer::Renderer* renderer) const;
		};
	}
}