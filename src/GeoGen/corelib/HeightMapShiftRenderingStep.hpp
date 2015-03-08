#pragma once

#include "../Direction.hpp"
#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapShiftRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Size1D maxDistance;
			Direction direction;
		public:
			HeightMapShiftRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Size1D maxDistance, Direction direction)
				: RenderingStep2D(location, argumentSlots, returnSlot), maxDistance(maxDistance), direction(direction) {};

			virtual String GetName() const { return GG_STR("HeightMap.Shift"); };

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}