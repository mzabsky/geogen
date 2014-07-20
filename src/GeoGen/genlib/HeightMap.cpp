#include "HeightMap.hpp"

using namespace geogen;
using namespace genlib;

HeightMap::HeightMap(Rectangle rectangle)
:rectangle(rectangle)
{
	this->heightData = new Height[rectangle.GetSize().GetTotalLength()];
	memset(this->heightData, 0, rectangle.GetSize().GetTotalLength());
}

HeightMap::~HeightMap()
{
	delete[] this->heightData;
}