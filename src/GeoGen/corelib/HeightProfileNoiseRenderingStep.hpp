#pragma once

#include "../Number.hpp"
#include "../genlib/NoiseLayersFactory.hpp"
#include "../random/RandomSeed.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileNoiseRenderingStep : public renderer::RenderingStep1D
		{
		private:
			Size1D waveLength;
			Height amplitude;
			random::RandomSeed seed;
			unsigned seedStep;
		public:
			HeightProfileNoiseRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Size1D waveLength, Height amplitude, random::RandomSeed seed, unsigned seedStep)
				: RenderingStep1D(location, argumentSlots, returnSlot), waveLength(waveLength), amplitude(amplitude), seed(seed), seedStep(seedStep) {};

			virtual String GetName() const { return GG_STR("HeightProfile.NoiseLayer"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}