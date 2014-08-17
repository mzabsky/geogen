#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapConvexityMapRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Size1D radius;
		public:
			HeightMapConvexityMapRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Size1D radius)
				: RenderingStep2D(location, argumentSlots, returnSlot), radius(radius) {};

			virtual String GetName() const { return GG_STR("HeightMap.ConvexityMap"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual Rectangle CalculateRenderingBounds(renderer::Renderer* renderer, Rectangle argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}