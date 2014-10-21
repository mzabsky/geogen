#include "CommonProfileFactory.hpp"

using namespace std;
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

HeightProfile CommonProfileFactory::CreateStratificationProfile(Size1D size, unsigned numberOfStrata, double steepness, double smoothness)
{
	double flatStepFactor = 0.5 + 0.5 * steepness;
	double steepStepFactor = 1 - flatStepFactor;
	double totalStepFactor = (flatStepFactor + steepStepFactor) * numberOfStrata + flatStepFactor;
	double stratumLength = size / totalStepFactor;
	double flatStepLength = flatStepFactor * stratumLength;
	double steepStepLength = steepStepFactor * stratumLength;

	double stratumHeight = HEIGHT_MAX / double(numberOfStrata);
	double flatStepHeight = (1 - flatStepFactor) * stratumHeight;
	double steepStepHeight = (1 - steepStepFactor) * stratumHeight;

	HeightProfile profile(Interval(0, size), 0);
	
	double currentPosition = 0;
	double currentHeight = 0;
	for (unsigned i = 0; i < numberOfStrata; i++)
	{
		profile.Gradient(Coordinate(currentPosition), Coordinate(currentPosition + flatStepLength), Height(currentHeight), Height(currentHeight + flatStepHeight), false);
		currentPosition += flatStepLength;
		currentHeight += flatStepHeight;

		profile.Gradient(Coordinate(currentPosition), Coordinate(currentPosition + steepStepLength), Height(currentHeight), Height(currentHeight + steepStepHeight), false);
		currentPosition += steepStepLength;
		currentHeight += steepStepHeight;
	}

	profile.Gradient(Coordinate(currentPosition), size - 1, Height(currentHeight), HEIGHT_MAX, false);
	profile.Blur(Size1D(10 * smoothness));

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