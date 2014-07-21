#include "HeightProfile.hpp"

using namespace geogen;
using namespace genlib;

HeightProfile::HeightProfile(Interval interval)
:interval(interval)
{
	this->heightData = new Height[interval.GetLength()];
	memset(this->heightData, 0, interval.GetLength());
}

HeightProfile::HeightProfile(HeightProfile const& other)
{
	this->interval = other.interval;

	this->heightData = new Height[this->interval.GetLength()];
	memcpy(this->heightData, other.heightData, sizeof(Height) * this->interval.GetLength());
}

HeightProfile& HeightProfile::operator=(HeightProfile& other)
{
	this->interval = other.interval;

	delete[] this->heightData;

	this->heightData = new Height[this->interval.GetLength()];
	memcpy(this->heightData, other.heightData, sizeof(Height) * this->interval.GetLength());

	return *this;
}

HeightProfile::~HeightProfile()
{
	delete[] this->heightData;
}