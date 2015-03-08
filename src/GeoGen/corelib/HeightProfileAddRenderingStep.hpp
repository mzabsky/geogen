#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileAddRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Height addend;
		public:
			HeightProfileAddRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Height addend)
				: RenderingStep1D(location, argumentSlots, returnSlot), addend(addend) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Add"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}