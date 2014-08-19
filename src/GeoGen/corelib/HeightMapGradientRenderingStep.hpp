#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapGradientRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Point source, destination;
			Height fromHeight, toHeight;
		public:
			HeightMapGradientRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Point source, Point destination, Height fromHeight, Height toHeight)
				: RenderingStep2D(location, argumentSlots, returnSlot), source(source), destination(destination), fromHeight(fromHeight), toHeight(toHeight) {};

			virtual String GetName() const { return GG_STR("HeightMap.Gradient"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}