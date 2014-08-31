#pragma once

#include "../Number.hpp"
#include "DataObject.hpp"

namespace geogen
{
	namespace genlib
	{
		class NoiseLayer
		{
		private:
			unsigned waveLength;
			unsigned amplitude;
		public:
			NoiseLayer() : waveLength(0), amplitude(0) {};
			NoiseLayer(unsigned waveLength, unsigned amplitude) : waveLength(waveLength), amplitude(amplitude) {};
			NoiseLayer(NoiseLayer const& other) { this->waveLength = other.waveLength; this->amplitude = other.amplitude; }
			NoiseLayer& operator=(NoiseLayer& other) { this->waveLength = other.waveLength; this->amplitude = other.amplitude; return *this; }

			inline unsigned GetWaveLength() const { return this->waveLength; }
			inline unsigned GetAmplitude() const { return this->amplitude; }
		};
	}
}