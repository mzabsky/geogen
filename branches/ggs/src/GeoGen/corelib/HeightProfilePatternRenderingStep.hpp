#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfilePatternRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Interval repeatInterval;
		public:
			HeightProfilePatternRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Interval repeatInterval)
				: RenderingStep1D(location, argumentSlots, returnSlot), repeatInterval(repeatInterval) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Pattern"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}