#include "HeightProfile.hpp"

using namespace geogen;
using namespace genlib;

HeightProfile::HeightProfile(int origin, int length)
:origin(origin), length(length)
{
	this->heightData = new Height[length];
}

HeightProfile::~HeightProfile()
{
	delete[] this->heightData;
}