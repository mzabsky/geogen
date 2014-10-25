#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapInvertRenderingStep : public renderer::RenderingStep2D
		{
		private:
		public:
			HeightMapInvertRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot)
				: RenderingStep2D(location, argumentSlots, returnSlot) {};

			virtual String GetName() const { return GG_STR("HeightMap.Invert"); };

			virtual void Step(renderer::Renderer* renderer) const;
		};
	}
}