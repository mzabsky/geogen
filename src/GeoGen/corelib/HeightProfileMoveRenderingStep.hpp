#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileMoveRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Coordinate offset;
		public:
			HeightProfileMoveRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Coordinate offset)
				: RenderingStep1D(location, argumentSlots, returnSlot), offset(offset) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Move"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual Interval CalculateRenderingBounds(renderer::Renderer* renderer, Interval argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}