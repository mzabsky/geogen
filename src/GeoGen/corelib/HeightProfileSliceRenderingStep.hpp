#pragma once

#include "../Number.hpp"
#include "../Direction.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileSliceRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Direction direction;
			Coordinate coordinate;
			Height heightFrom, heightTo;
		public:
			HeightProfileSliceRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Direction direction, Coordinate coordinate)
				: RenderingStep1D(location, argumentSlots, returnSlot), direction(direction), coordinate(coordinate) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Slice"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual unsigned GetPeakExtraMemory(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds const*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}