#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"
#include "../Rectangle.hpp"

namespace geogen
{
	namespace corelib
	{
		class YieldRenderingStep : public renderer::RenderingStep2D
		{
		private:
			String name;
			Rectangle rect;
		public:
			YieldRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, String name, Rectangle rect)
				: RenderingStep2D(location, argumentSlots, returnSlot), name(name), rect(rect) {
			};

			virtual String GetName() const { return GG_STR("Yield"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual Rectangle CalculateRenderingBounds(renderer::Renderer* renderer, Rectangle argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}