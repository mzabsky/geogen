#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapFillRectangleRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Rectangle fillRectangle;
			Height height;
		public:
			HeightMapFillRectangleRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Rectangle fillRectangle, Height height)
				: RenderingStep2D(location, argumentSlots, returnSlot), fillRectangle(fillRectangle), height(height) {};

			virtual String GetName() const { return GG_STR("HeightMap.FillRectangle"); };

			virtual void Step(renderer::Renderer* renderer) const;
		};
	}
}