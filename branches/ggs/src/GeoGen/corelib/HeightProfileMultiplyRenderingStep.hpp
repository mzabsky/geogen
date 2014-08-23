#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileMultiplyRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Height factor;
		public:
			HeightProfileMultiplyRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height factor)
				: RenderingStep1D(location, argumentSlots, returnSlot), factor(factor) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Multiply"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}