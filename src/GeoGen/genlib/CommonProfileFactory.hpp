#pragma once

#include <map>

#include "HeightProfile.hpp"

namespace geogen
{
	namespace genlib
	{
		typedef std::map<Size1D, Height> NoiseLayers;

		/// Factory class creating well known HeightProfile instances.
		class CommonProfileFactory
		{
		private:
			// This is a static class, instance creation is forbidden.
			CommonProfileFactory(){};
		public:
			static HeightProfile CreateGlaciationProfile(Size1D size, double strength);
			static HeightProfile CreateStratificationProfile(Size1D size, unsigned numberOfStrata, double steepness, double smoothness);

			static HeightProfile CreateMirroredProfile(HeightProfile& profile);
		};
	}
}
