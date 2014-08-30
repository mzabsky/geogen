#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapPatternRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Rectangle repeatRectangle;
		public:
			HeightMapPatternRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Rectangle repeatRectangle)
				: RenderingStep2D(location, argumentSlots, returnSlot), repeatRectangle(repeatRectangle) {};

			virtual String GetName() const { return GG_STR("HeightMap.Pattern"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;
		};
	}
}