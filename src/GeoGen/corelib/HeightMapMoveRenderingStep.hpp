#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapMoveRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Point offset;
		public:
			HeightMapMoveRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Point offset)
				: RenderingStep2D(location, argumentSlots, returnSlot), offset(offset) {};

			virtual String GetName() const { return GG_STR("HeightMap.Move"); };

			virtual Rectangle CalculateRenderingBounds(renderer::Renderer* renderer, Rectangle argumentBounds) const;

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}