#pragma once

#include <map>

#include "HeightProfile.hpp"

namespace geogen
{
	namespace genlib
	{
		typedef std::map<Size1D, Height> NoiseLayers;

		class CommonProfileFactory
		{
		private:
			// This is a static class, instance creation is forbidden.
			CommonProfileFactory(){};
		public:
			static HeightProfile CreateGlaciationProfile(Size1D size, double strength);

			static HeightProfile CreateMirroredProfile(HeightProfile& profile);
		};
	}
}