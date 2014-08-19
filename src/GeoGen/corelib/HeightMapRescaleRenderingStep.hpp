#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapRescaleRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Scale horizontalScale;
			Scale verticalScale;
		public:
			HeightMapRescaleRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Scale horizontalScale, Scale verticalScale)
				: RenderingStep2D(location, argumentSlots, returnSlot), horizontalScale(horizontalScale), verticalScale(verticalScale) {};

			virtual String GetName() const { return GG_STR("HeightMap.Rescale"); };

			virtual Rectangle CalculateRenderingBounds(renderer::Renderer* renderer, Rectangle argumentBounds) const;

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}