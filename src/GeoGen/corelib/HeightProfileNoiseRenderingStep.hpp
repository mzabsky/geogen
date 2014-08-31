#pragma once

#include "../Number.hpp"
#include "../genlib/NoiseLayer.hpp"
#include "../random/RandomSeed.hpp"
#include "../renderer/RenderingStep1D.hpp"

namespace geogen
{
	namespace corelib
	{
		class HeightProfileNoiseRenderingStep : public renderer::RenderingStep1D
		{
		private:
			std::vector<genlib::NoiseLayer> noiseLayers;
			random::RandomSeed randomSeed;
		public:
			HeightProfileNoiseRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, std::vector<genlib::NoiseLayer> noiseLayers, random::RandomSeed randomSeed)
				: RenderingStep1D(location, argumentSlots, returnSlot), noiseLayers(noiseLayers), randomSeed(randomSeed) {};

			virtual String GetName() const { return GG_STR("HeightProfile.Noise"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}