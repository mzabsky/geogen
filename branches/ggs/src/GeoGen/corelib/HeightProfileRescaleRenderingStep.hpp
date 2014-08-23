#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileRescaleRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Scale scale;
		public:
			HeightProfileRescaleRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Scale scale)
				: RenderingStep1D(location, argumentSlots, returnSlot), scale(scale) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Rescale"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual Interval HeightProfileRescaleRenderingStep::CalculateRenderingBounds(renderer::Renderer* renderer, Interval argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}