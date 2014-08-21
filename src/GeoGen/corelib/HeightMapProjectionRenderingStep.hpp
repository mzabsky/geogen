#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"
#include "../Orientation.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapProjectionRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Orientation orientation;
		public:
			HeightMapProjectionRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Orientation orientation)
				: RenderingStep2D(location, argumentSlots, returnSlot), orientation(orientation) {};

			virtual String GetName() const { return GG_STR("HeightMap.Projection"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}