#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapMultiplyMapRenderingStep : public renderer::RenderingStep2D
		{
		private:
		public:
			HeightMapMultiplyMapRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot)
				: RenderingStep2D(location, argumentSlots, returnSlot) {};

			virtual String GetName() const { return GG_STR("HeightMap.MultiplyMap"); };

			virtual void Step(renderer::Renderer* renderer) const;
		};
	}
}