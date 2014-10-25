#pragma once

#include "../Interval.hpp"
#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapTransformHeightsRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Interval interval;
			Height minHeight;
			Height maxHeight;
		public:
			HeightMapTransformHeightsRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Interval interval, Height minHeight, Height maxHeight)
				: RenderingStep2D(location, argumentSlots, returnSlot), interval(interval), minHeight(minHeight), maxHeight(maxHeight) {};

			virtual String GetName() const { return GG_STR("HeightMap.TransformHeights"); };

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}