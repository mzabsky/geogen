#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapRadialGradientRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Point point;
			Height radius;
			Height fromHeight;
			Height toHeight;
		public:
			HeightMapRadialGradientRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Point point, Size1D radius, Height fromHeight, Height toHeight)
				: RenderingStep2D(location, argumentSlots, returnSlot), point(point), radius(radius), fromHeight(fromHeight), toHeight(toHeight) {};

			virtual String GetName() const { return GG_STR("HeightMap.RadialGradient"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}