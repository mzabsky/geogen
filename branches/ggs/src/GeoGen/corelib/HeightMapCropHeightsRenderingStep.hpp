#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapCropHeightsRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Height min;
			Height max;
			Height replace;
		public:
			HeightMapCropHeightsRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height min, Height max, Height replace)
				: RenderingStep2D(location, argumentSlots, returnSlot), min(min), max(max), replace(replace) {};

			virtual String GetName() const { return GG_STR("HeightMap.CropHeights"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}