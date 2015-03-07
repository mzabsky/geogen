#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileGradientRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Coordinate source, destination;
			Height heightFrom, heightTo;
		public:
			HeightProfileGradientRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Coordinate source, Coordinate destination, Height heightFrom, Height heightTo)
				: RenderingStep1D(location, argumentSlots, returnSlot), source(source), destination(destination), heightFrom(heightFrom), heightTo(heightTo) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Gradient"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual unsigned GetPeakExtraMemory(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds const*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}