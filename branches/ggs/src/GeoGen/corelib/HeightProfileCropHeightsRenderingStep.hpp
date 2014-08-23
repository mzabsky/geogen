#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileCropHeightsRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Height minHeight, maxHeight, replace;
		public:
			HeightProfileCropHeightsRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height minHeight, Height maxHeight, Height replace)
				: RenderingStep1D(location, argumentSlots, returnSlot), minHeight(minHeight), maxHeight(maxHeight), replace(replace) {};

			virtual String GetName() const { return GG_STR("HeightProfile.CropHeights"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}