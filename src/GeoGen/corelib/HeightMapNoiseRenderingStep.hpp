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
			genlib::NoiseLayers layers;
			random::RandomSeed seed;
		public:
			HeightMapNoiseRenderingStep(CodeLocation location, std::vector<unsigned> const& argumentSlots, unsigned returnSlot, genlib::NoiseLayers const& layers, random::RandomSeed seed)
				: RenderingStep2D(location, argumentSlots, returnSlot), layers(layers), seed(seed) {};

			virtual String GetName() const { return GG_STR("HeightMap.Noise"); };

			virtual void Step(renderer::Renderer* renderer) const;

			virtual void SerializeArguments(IOStream& stream) const;
		};
	}
}