#include "NoiseLayersFactory.hpp"

using namespace geogen;
using namespace genlib;

NoiseLayers NoiseLayersFactory::CreateDefaultLayers()
{
	Size1D waveLength = 256;
	Height amplitude = HEIGHT_MAX / 2;

	NoiseLayers layers;
	while (waveLength > 0)
	{
		layers[waveLength] = amplitude;

		waveLength /= 2;
		amplitude /= 2;
	}

	return layers;
}