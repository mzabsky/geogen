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

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual unsigned GetPeakExtraMemory(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds const*> argumentBounds) const;

			virtual void SimulateOnRenderingBounds(renderer::RenderingBounds* renderingBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}