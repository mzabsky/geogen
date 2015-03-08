#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapDrawLineRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Point start;
			Point end;
			Height height;
		public:
			HeightMapDrawLineRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Point start, Point end, Height height)
				: RenderingStep2D(location, argumentSlots, returnSlot), start(start), end(end), height(height) {};

			virtual String GetName() const { return GG_STR("HeightMap.DrawLine"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}