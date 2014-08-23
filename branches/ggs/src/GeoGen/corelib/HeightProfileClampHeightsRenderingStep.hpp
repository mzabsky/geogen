#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileClampHeightsRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Height minHeight, maxHeight;
		public:
			HeightProfileClampHeightsRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height minHeight, Height maxHeight)
				: RenderingStep1D(location, argumentSlots, returnSlot), minHeight(minHeight), maxHeight(maxHeight) {};

			virtual String GetName() const { return GG_STR("HeightProfile.ClampHeights"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}