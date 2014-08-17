#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapClampHeightsRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Height min;
			Height max;
		public:
			HeightMapClampHeightsRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height min, Height max)
				: RenderingStep2D(location, argumentSlots, returnSlot), min(min), max(max) {};

			virtual String GetName() const { return GG_STR("HeightMap.ClampHeights"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}