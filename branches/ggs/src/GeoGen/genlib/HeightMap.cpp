#include <vector>

#include "HeightMap.hpp"
#include "../ApiUsageException.hpp"
#include "../random/RandomSequence2D.hpp"

using namespace geogen;
using namespace genlib;
using namespace random;
using namespace std;

#define FOR_EACH_IN_RECT(x, y, rect) \
	for (Coordinate y = rect.GetPosition().GetY(); y < rect.GetEndingPoint().GetY(); y++) \
		for (Coordinate x = rect.GetPosition().GetX(); x < rect.GetEndingPoint().GetX(); x++) \

HeightMap::HeightMap(Rectangle rectangle, Height height, Scale scale)
:rectangle(rectangle), scale(scale)
{
	this->heightData = new Height[rectangle.GetSize().GetTotalLength()];
	
	Rectangle physicalRect = this->GetPhysicalRectangleUnscaled(rectangle);
	FOR_EACH_IN_RECT(x, y, physicalRect)
	{
		(*this)(x, y) = height;
	}
}

HeightMap::HeightMap(HeightMap const& other)
{
	this->rectangle = other.rectangle;
	this->scale = other.scale;

	this->heightData = new Height[this->rectangle.GetSize().GetTotalLength()];
	memcpy(this->heightData, other.heightData, sizeof(Height) * this->rectangle.GetSize().GetTotalLength());
}

HeightMap::HeightMap(HeightMap const& other, Rectangle cutoutRect)
{
	this->rectangle = cutoutRect;
	this->heightData = new Height[cutoutRect.GetSize().GetTotalLength()];
	this->scale = other.scale;

	Rectangle physicalRect = this->GetPhysicalRectangleUnscaled(cutoutRect);

	// Fill all pixels with 0, because the cuout rect might have only partially overlapped with the original rect.
	FOR_EACH_IN_RECT(x, y, physicalRect)
	{
		(*this)(x, y) = 0;
	}

	Rectangle intersection = this->GetPhysicalRectangleUnscaled(Rectangle::Intersect(other.rectangle, cutoutRect));

	Point offset = cutoutRect.GetPosition() - other.rectangle.GetPosition();
	FOR_EACH_IN_RECT(x, y, intersection)
	{
		(*this)(x, y) = other(x + offset.GetX(), y + offset.GetY());
	}
	
	memcpy(this->heightData, other.heightData, sizeof(Height)* this->rectangle.GetSize().GetTotalLength());
}

HeightMap& HeightMap::operator=(HeightMap& other)
{
	this->rectangle = other.rectangle;
	this->scale = other.scale;

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
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(Rectangle::Intersect(this->rectangle, addend->rectangle));

	Point offset = addend->GetRectangle().GetPosition() - this->rectangle.GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRect) 
	{
		(*this)(x, y) += (*addend)(x + offset.GetX(), y + offset.GetY());
	}
}

void HeightMap::RadialGradient(Point point, Size1D radius, Height fromHeight, Height toHeight)
{
	Point physicalCenter = this->GetPhysicalPoint(point);
	Size1D scaledRadius = Size1D(radius * this->scale);

	Rectangle physicalRect = this->GetPhysicalRectangleUnscaled(this->rectangle);
	FOR_EACH_IN_RECT(x, y, physicalRect)
	{
		unsigned long long distance = physicalCenter.GetDistanceTo(Point(x, y));

		if (distance > scaledRadius)
		{
			(*this)(x, y) = toHeight;
		}
		else
		{
			(*this)(x, y) = fromHeight + (Height)(((long long)toHeight - (long long)fromHeight) * (long long)distance / (long long)scaledRadius);
		}
	}
}

void HeightMap::Blur(Size1D radius)
{
	this->Blur(radius, ORIENTATION_HORIZONTAL);
	this->Blur(radius, ORIENTATION_VERTICAL);
}

void HeightMap::Blur(Size1D radius, Orientation direction)
{
	// Allocate the new array.
	Height* new_data = new Height[this->rectangle.GetSize().GetTotalLength()];

	Size1D scaledRadius = Size1D(radius * this->scale);

	if (direction == ORIENTATION_HORIZONTAL) {
		for (Coordinate y = 0; y < (Coordinate)this->GetHeight(); y++) {
			// Prefill the window with value of the left edge + n leftmost values (where n is kernel size).
			Size1D window_size = scaledRadius * 2 + 1;
			long long window_value = (*this)(0, y) * scaledRadius;

			for (long long x = 0; x < scaledRadius; x++) {
				window_value += (*this)(x, y);
			}

			/* In every step shift the window one tile to the right  (= subtract its leftmost cell and add
			value of rightmost + 1). i represents position of the central cell of the window. */
			for (Coordinate x = 0; x < this->GetWidth(); x++) {
				// If the window is approaching right border, use the rightmost value as fill.
				if (x < scaledRadius) {
					window_value += (*this)(x + scaledRadius, y) - (*this)(0, y);
				}
				else if (x + scaledRadius < this->GetWidth()) {
					window_value += (*this)(x + scaledRadius, y) - (*this)(x - scaledRadius, y);
				}
				else {
					window_value += (*this)(this->GetWidth() - 1, y) - (*this)(x - scaledRadius, y);
				}

				// Set the value of current tile to arithmetic average of window tiles.
				new_data[x + this->GetWidth() * y] = window_value / window_size;
			}
		}
	}
	else { 
		for (Coordinate x = 0; x < this->GetHeight(); x++) {
			// Prefill the window with value of the left edge + n topmost values (where n is radius).
			Size1D window_size = scaledRadius * 2 + 1;
			long long window_value = (*this)(x, 0) * scaledRadius;

			for (Size1D y = 0; y < scaledRadius; y++) {
				window_value += (*this)(x, y);
			}

			/* In every step shift the window one tile to the bottom  (= subtract its topmost cell and add
			value of bottommost + 1). i represents position of the central cell of the window. */
			for (Coordinate y = 0; y < this->GetHeight(); y++) {
				// If the window is approaching right border, use the rightmost value as fill.
				if (y < scaledRadius) {
					window_value += (*this)(x, y + scaledRadius) - (*this)(x, 0);
				}
				else if (y + scaledRadius < this->GetHeight()) {
					window_value += (*this)(x, y + scaledRadius) - (*this)(x, y - scaledRadius);
				}
				else {
					window_value += (*this)(x, this->GetHeight() - 1) - (*this)(x, y - scaledRadius);
				}

				// Set the value of current tile to arithmetic average of window tiles. 
				new_data[x + this->GetWidth() * y] = window_value / window_size;
			}
		}
	}

	// Relink and delete the original array data
	delete[] this->heightData;
	this->heightData = new_data;
}

void HeightMap::FillRectangle(Rectangle fillRectangle, Height height)
{
	Rectangle operationRect = Rectangle::Intersect(this->GetPhysicalRectangleUnscaled(this->rectangle), this->GetPhysicalRectangle(fillRectangle));
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = height;
	}
}

void HeightMap::Noise(std::vector<NoiseLayer> layers, RandomSeed seed)
{
	this->FillRectangle(RECTANGLE_MAX, 0);

	RandomSequence2D randomSequence(seed);

	Size1D horizontalBufferSize = this->rectangle.GetSize().GetWidth() + 4;
	Size1D verticalBufferSize = this->rectangle.GetSize().GetHeight();

	vector<Height> topBorderBuffer(horizontalBufferSize * 2, 0);
	vector<Height> bottomBorderBuffer(horizontalBufferSize * 2, 0);
	vector<Height> leftBorderBuffer(verticalBufferSize * 2, 0);
	vector<Height> rightBorderBuffer(verticalBufferSize * 2, 0);

	unsigned maxLevel = 5;
	unsigned initialWaveLength = 1 << maxLevel;

	Point topBorderBufferOrigin(
		PreviousMultipleOfExclusive(this->rectangle.GetPosition().GetX(), initialWaveLength) - initialWaveLength,
		PreviousMultipleOfExclusive(this->rectangle.GetPosition().GetY(), initialWaveLength) - initialWaveLength);

	Point bottomBorderBufferOrigin(
		PreviousMultipleOfExclusive(this->rectangle.GetPosition().GetX(), initialWaveLength) - initialWaveLength,
		NextMultipleOfInclusive(this->rectangle.GetPosition().GetY(), initialWaveLength) + initialWaveLength);

	Point leftBorderBufferOrigin(
		PreviousMultipleOfExclusive(this->rectangle.GetPosition().GetX(), initialWaveLength) - initialWaveLength,
		NextMultipleOfInclusive(this->rectangle.GetPosition().GetY(), initialWaveLength));

	Point rightBorderBufferOrigin(
		NextMultipleOfExclusive(this->rectangle.GetPosition().GetX(), initialWaveLength),
		NextMultipleOfInclusive(this->rectangle.GetPosition().GetY(), initialWaveLength));

	for (Coordinate logicalY = NextMultipleOfInclusive(this->rectangle.GetPosition().GetY(), initialWaveLength); logicalY < this->rectangle.GetEndingPoint().GetY(); logicalY += initialWaveLength)
	{
		for (Coordinate logicalX = NextMultipleOfInclusive(this->rectangle.GetPosition().GetX(), initialWaveLength); logicalX < this->rectangle.GetEndingPoint().GetX(); logicalX += initialWaveLength)
		{
			Point logicalPoint(logicalX, logicalY);
			(*this)(this->GetPhysicalPoint(logicalPoint)) = randomSequence.GetHeight(logicalPoint);
		}
	}

	// Initialize the horizontal buffers
	for (unsigned bufferY = 0; bufferY < 2; bufferY++)
	{
		for (unsigned bufferX = 0; bufferX < horizontalBufferSize; bufferX++)
		{
			Point topLogicalPoint = topBorderBufferOrigin + Point(bufferX * initialWaveLength, bufferY * initialWaveLength);
			(*this)(this->GetPhysicalPoint(topLogicalPoint)) = randomSequence.GetHeight(topLogicalPoint);

			Point bottomLogicalPoint = bottomBorderBufferOrigin + Point(bufferX * initialWaveLength, bufferY * initialWaveLength);
			(*this)(this->GetPhysicalPoint(bottomLogicalPoint)) = randomSequence.GetHeight(bottomLogicalPoint);
		}
	}

	// Initialize the vertical buffers
	for (unsigned bufferY = 0; bufferY < verticalBufferSize; bufferY++)
	{
		for (unsigned bufferX = 0; bufferX < 2; bufferX++)
		{
			Point leftLogicalPoint = topBorderBufferOrigin + Point(bufferX * initialWaveLength, bufferY * initialWaveLength);
			(*this)(this->GetPhysicalPoint(leftLogicalPoint)) = randomSequence.GetHeight(leftLogicalPoint);

			Point rightLogicalPoint = bottomBorderBufferOrigin + Point(bufferX * initialWaveLength, bufferY * initialWaveLength);
			(*this)(this->GetPhysicalPoint(rightLogicalPoint)) = randomSequence.GetHeight(rightLogicalPoint);
		}
	}

	for (unsigned level = maxLevel; level >= 0; level--)
	{
		unsigned waveLength = 1 << level;
		unsigned amplitude = 1 << level;

		// The square step - put a randomly generated point into center of each square.
		Point bottomBufferEndingPoint = bottomBorderBufferOrigin + Point(horizontalBufferSize * waveLength, 2 * waveLength);
		for (Coordinate logicalY = topBorderBufferOrigin.GetY(); logicalY <= bottomBufferEndingPoint.GetX(); logicalY += waveLength)
		{
			for (Coordinate logicalX = topBorderBufferOrigin.GetX(); logicalX <= bottomBufferEndingPoint.GetX(); logicalX += waveLength)
			{

			}
		}
	}

	FOR_EACH_IN_RECT(logicalX, logicalY, this->rectangle)
	{
		Point logicalPoint(logicalX, logicalY);
		Point physicalPoint = this->GetPhysicalPointUnscaled(logicalPoint);

		(*this)(physicalPoint) = randomSequence.GetHeight(logicalPoint);
	}

	
}