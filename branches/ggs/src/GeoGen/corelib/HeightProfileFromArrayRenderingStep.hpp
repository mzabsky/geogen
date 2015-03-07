#pragma once

#include <map>

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileFromArrayRenderingStep : public renderer::RenderingStep1D
		{
		private:
			std::map<Coordinate, Height> heights;
		public:
			HeightProfileFromArrayRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, std::map<Coordinate, Height> const& heights)
				: RenderingStep1D(location, argumentSlots, returnSlot), heights(heights) {};

			virtual String GetName() const { return GG_STR("HeightProfile.FromArray"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual unsigned GetPeakExtraMemory(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds const*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}