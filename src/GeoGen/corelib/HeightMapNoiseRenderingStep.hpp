#pragma once

#include <map>

#include "../Number.hpp"
#include "../renderer/RenderingStep2D.hpp"
#include "../random/RandomSeed.hpp"
#include "../genlib/NoiseLayersFactory.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightMapNoiseRenderingStep : public renderer::RenderingStep2D
		{
		private:
			Size1D waveLength;
			Height amplitude;
			random::RandomSeed seed;
			unsigned seedStep;
		public:
			HeightMapNoiseRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, Size1D waveLength, Height amplitude, random::RandomSeed seed, unsigned seedStep)
				: RenderingStep2D(location, argumentSlots, returnSlot), waveLength(waveLength), amplitude(amplitude), seed(seed), seedStep(seedStep) {};

			virtual String GetName() const { return GG_STR("HeightMap.NoiseLayer"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}