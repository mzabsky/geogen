#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileBlurRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Size1D radius;
		public:
			HeightProfileBlurRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Size1D radius)
				: RenderingStep1D(location, argumentSlots, returnSlot), radius(radius) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Blur"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void UpdateRenderingBounds(renderer::Renderer* renderer, std::vector<renderer::RenderingBounds*> argumentBounds) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}