#include "HeightMap.hpp"

using namespace geogen;
using namespace genlib;

HeightMap::HeightMap(Rectangle rectangle)
:rectangle(rectangle)
{
	this->heightData = new Height[rectangle.GetSize().GetTotalLength()];
	memset(this->heightData, 0, rectangle.GetSize().GetTotalLength());
}

HeightMap::HeightMap(HeightMap const& other)
{
	this->rectangle = other.rectangle;
	
	this->heightData = new Height[this->rectangle.GetSize().GetTotalLength()];
	memcpy(this->heightData, other.heightData, sizeof(Height) * this->rectangle.GetSize().GetTotalLength());
}

HeightMap& HeightMap::operator=(HeightMap& other)
{
	this->rectangle = other.rectangle;

	delete [] this->heightData;

	this->heightData = new Height[this->rectangle.GetSize().GetTotalLength()];
	memcpy(this->heightData, other.heightData, sizeof(Height) * this->rectangle.GetSize().GetTotalLength());

	return *this;
}

HeightMap::~HeightMap()
{
	delete[] this->heightData;
}