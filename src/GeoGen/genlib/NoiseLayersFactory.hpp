#pragma once

#include <map>

#include "../Number.hpp"

namespace geogen
{
	namespace genlib
	{
		typedef std::map<Size1D, Height> NoiseLayers;

		class NoiseLayersFactory
		{
		private:
			// This is a static class, instance creation is forbidden.
			NoiseLayersFactory(){};
		public:
			static NoiseLayers CreateDefaultLayers();
		};
	}
}