#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileFlatRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Height height;
		public:
			HeightProfileFlatRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height height)
				: RenderingStep1D(location, argumentSlots, returnSlot), height(height) {};
			
			virtual String GetName() const { return GG_STR("HeightProfile.Flat"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual unsigned GetPeakExtraMemory(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds const*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}