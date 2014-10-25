#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapCropRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Rectangle rectangle;
			Height height;
		public:
			HeightMapCropRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Rectangle rectangle, Height height)
				: RenderingStep2D(location, argumentSlots, returnSlot), rectangle(rectangle), height(height) {};

			virtual String GetName() const { return GG_STR("HeightMap.Crop"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}