#include "HeightMap.hpp"
#include "../ApiUsageException.hpp"

using namespace geogen;
using namespace genlib;

#define FOR_EACH_IN_RECT(x, y, rect) \
	for (Coordinate y = 0; y < rect.GetSize().GetWidth(); y++) \
		for (Coordinate x = 0; x < rect.GetSize().GetWidth(); x++) \

HeightMap::HeightMap(Rectangle rectangle, Height height)
:rectangle(rectangle)
{
	this->heightData = new Height[rectangle.GetSize().GetTotalLength()];
	
	FOR_EACH_IN_RECT(x, y, rectangle)
	{
		(*this)(x, y) = height;
	}
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

void HeightMap::AddMap(HeightMap* addend)
{
	if (!this->rectangle.Contains(addend->GetRectangle()))
	{
		throw ApiUsageException("The second addend must be contained by the first addend.");
	}

	Rectangle operationRect = Rectangle::Intersect(this->rectangle, addend->rectangle) - (addend->rectangle.GetPosition() - this->rectangle.GetPosition());

	FOR_EACH_IN_RECT(x, y, operationRect) 
	{
		(*this)(x, y) += (*addend)(x, y);
	}
}