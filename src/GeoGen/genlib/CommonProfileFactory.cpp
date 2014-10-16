#include "CommonProfileFactory.hpp"

using namespace geogen;
using namespace genlib;

HeightProfile CommonProfileFactory::CreateGlaciationProfile(Size1D size, double strength)
{
	HeightProfile profile(Interval(0, size), 0);

	profile(0) = 0;
	for (unsigned i = 1; i < size; i++)
	{
		profile((Coordinate)i) = 1 + pow(1 - sin(((size - 1 - i) / double(size) * NUMBER_PI) / 2), 0.7 + 2 * strength) * double(HEIGHT_MAX - 1);
	}

	return profile;
}

HeightProfile CommonProfileFactory::CreateMirroredProfile(HeightProfile& profile)
{
	HeightProfile ret(Interval(0, profile.GetLength() * 2 - 1), 0);

	for (unsigned i = 0; i < profile.GetLength(); i++)
	{
		ret(Coordinate(profile.GetLength() - 1 + i)) = profile(Coordinate(i));
		ret(Coordinate(profile.GetLength() - 1 - i)) = -profile(Coordinate(i));
	}

	return ret;
}