#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"
#include "../Direction.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapProjectionRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Direction direction;
		public:
			HeightMapProjectionRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Direction direction)
				: RenderingStep2D(location, argumentSlots, returnSlot), direction(direction) {};

			virtual String GetName() const { return GG_STR("HeightMap.Projection"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}