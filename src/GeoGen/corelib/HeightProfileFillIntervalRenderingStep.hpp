#pragma once

#include "../Number.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileFillIntervalRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Interval interval;
			Height height;
		public:
			HeightProfileFillIntervalRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Interval interval, Height height)
				: RenderingStep1D(location, argumentSlots, returnSlot), interval(interval), height(height) {};

			virtual String GetName() const { return GG_STR("HeightProfile.FillInterval"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}