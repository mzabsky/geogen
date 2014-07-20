#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapFlatRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Height height;
		public:
			HeightMapFlatRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height height)
				: RenderingStep2D(location, argumentSlots, returnSlot), height(height) {};

			virtual String GetName() const { return GG_STR("HeightMap.Flat"); };

			virtual void Step(renderer::Renderer* renderer) const;
		};
	}
}