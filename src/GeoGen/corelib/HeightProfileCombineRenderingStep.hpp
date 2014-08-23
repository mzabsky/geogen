#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileCombineRenderingStep : public renderer::RenderingStep1D
		{
		private:
		public:
			HeightProfileCombineRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot)
				: RenderingStep1D(location, argumentSlots, returnSlot) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Combine"); };

			virtual void Step(renderer::Renderer* renderer) const;
		};
	}
}