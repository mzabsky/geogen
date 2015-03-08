#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapMultiplyRenderingStep : public renderer::RenderingStep2D
		{
		private:
			double factor;
		public:
			HeightMapMultiplyRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, double factor)
				: RenderingStep2D(location, argumentSlots, returnSlot), factor(factor) {};

			virtual String GetName() const { return GG_STR("HeightMap.Multiply"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}