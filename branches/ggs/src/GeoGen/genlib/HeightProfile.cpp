#include "HeightProfile.hpp"

using namespace geogen;
using namespace genlib;

HeightProfile::HeightProfile(Interval interval)
:interval(interval)
{
	this->heightData = new Height[interval.GetLength()];
}

HeightProfile::~HeightProfile()
{
	delete[] this->heightData;
}