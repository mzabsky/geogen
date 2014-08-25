#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapRepeatRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Rectangle repeatRectangle;
		public:
			HeightMapRepeatRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Rectangle repeatRectangle)
				: RenderingStep2D(location, argumentSlots, returnSlot), repeatRectangle(repeatRectangle) {};

			virtual String GetName() const { return GG_STR("HeightMap.Repeat"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}