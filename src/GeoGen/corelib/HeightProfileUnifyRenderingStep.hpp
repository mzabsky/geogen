#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileUnifyRenderingStep : public renderer::RenderingStep1D
		{
		private:
		public:
			HeightProfileUnifyRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot)
				: RenderingStep1D(location, argumentSlots, returnSlot) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Unify"); };

			virtual void Step(renderer::Renderer* renderer) const;
		};
	}
}