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
			Height minHeight;
			Height maxHeight;
		public:
			HeightMapClampHeightsRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height minHeight, Height maxHeight)
				: RenderingStep2D(location, argumentSlots, returnSlot), minHeight(minHeight), maxHeight(maxHeight) {};

			virtual String GetName() const { return GG_STR("HeightMap.ClampHeights"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}