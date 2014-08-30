#include <vector>

#include "HeightMap.hpp"
#include "../ApiUsageException.hpp"
#include "../random/RandomSequence2D.hpp"
#include "HeightProfile.hpp"
#include "../InternalErrorException.hpp"

using namespace geogen;
using namespace genlib;
using namespace random;
using namespace std;

#define FOR_EACH_IN_RECT(x, y, rect) \
	for (Coordinate y = rect.GetPosition().GetY(); y < rect.GetEndingPoint().GetY(); y++) \
		for (Coordinate x = rect.GetPosition().GetX(); x < rect.GetEndingPoint().GetX(); x++)

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

void HeightMap::Abs()
{
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = (*this)(x, y) > 0 ? (*this)(x, y) : -(*this)(x, y);
	}
}

void HeightMap::Add(Height addend)
{
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) += addend;
	}
}

void HeightMap::AddMasked(Height addend, HeightMap* mask)
{
	if (!mask->rectangle.Contains(this->rectangle))
	{
		throw ApiUsageException(GG_STR("Mask is too small."));
	}

	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	Point maskOffset = mask->GetRectangle().GetPosition() - this->rectangle.GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		Height maskHeight = (*mask)(x + maskOffset.GetX(), y + maskOffset.GetY());
		(*this)(x, y) += addend * Height(maskHeight / (double)HEIGHT_MAX);
	}
}

void HeightMap::AddMap(HeightMap* addend)
{
	Rectangle intersection = Rectangle::Intersect(this->rectangle, addend->rectangle);
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(intersection);

	Point offset = this->rectangle.GetPosition() - intersection.GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRect) 
	{
		(*this)(x, y) += (*addend)(x + offset.GetX(), y + offset.GetY());
	}
}

void HeightMap::AddMapMasked(HeightMap* addend, HeightMap* mask)
{
	Rectangle intersection = Rectangle::Intersect(this->rectangle, addend->rectangle);
	
	if (!mask->rectangle.Contains(intersection))
	{
		throw ApiUsageException(GG_STR("Mask is too small."));
	}
	
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(intersection);

	Point addendOffset = this->rectangle.GetPosition() - intersection.GetPosition();
	Point maskOffset = mask->GetRectangle().GetPosition() - this->rectangle.GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		Height addendHeight = (*addend)(x + addendOffset.GetX(), y + addendOffset.GetY());
		Height maskHeight = (*mask)(x + maskOffset.GetX(), y + maskOffset.GetY());
		(*this)(x, y) += addendHeight * Height(maskHeight / (double)HEIGHT_MAX);
	}
}

void HeightMap::Blur(Size1D radius)
{
	this->Blur(radius, DIRECTION_HORIZONTAL);
	this->Blur(radius, DIRECTION_VERTICAL);
}

void HeightMap::Blur(Size1D radius, Direction direction)
{

	// Allocate the new array.
	Height* new_data = new Height[this->rectangle.GetSize().GetTotalLength()];

	Size1D scaledRadius = this->GetScaledSize(radius);

	if (direction == DIRECTION_HORIZONTAL) {
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

void HeightMap::ClampHeights(Height min, Height max)
{
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = std::min(max, std::max(min, (*this)(x, y)));
	}
}

void HeightMap::Combine(HeightMap* other, HeightMap* mask)
{
	Rectangle intersection = Rectangle::Intersect(this->rectangle, other->rectangle);

	if (!mask->rectangle.Contains(intersection))
	{
		throw ApiUsageException(GG_STR("Mask is too small."));
	}

	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(intersection);

	Point otherOffset = this->rectangle.GetPosition() - intersection.GetPosition();
	Point maskOffset = mask->GetRectangle().GetPosition() - this->rectangle.GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		Height thisHeight = (*this)(x, y);
		Height otherHeight = (*other)(x + otherOffset.GetX(), y + otherOffset.GetY());
		Height maskHeight = (*mask)(x + maskOffset.GetX(), y + maskOffset.GetY());
		double factor = maskHeight / (double)HEIGHT_MAX;
		(*this)(x, y) += Height(thisHeight * factor) + Height(otherHeight * (1 - factor));
	}
}

void HeightMap::ConvexityMap(Size1D radius)
{
	HeightMap unsmoothed(*this);

	/* Convexity map is a difference between the current map and its smoothed variant. Smoothing erases any terrain features
	that peak upwards (are convex) or bulge downwards (are concave). */
	this->Blur(radius);
	this->Invert();
	this->AddMap(&unsmoothed);
}

void HeightMap::CropHeights(Height min, Height max, Height replace)
{
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		if ((*this)(x, y) > max || (*this)(x, y) < min)
		{
			(*this)(x, y) = replace;
		}
	}
}

void HeightMap::FillRectangle(Rectangle fillRectangle, Height height)
{
	Rectangle operationRect = Rectangle::Intersect(this->GetPhysicalRectangleUnscaled(this->rectangle), this->GetPhysicalRectangle(fillRectangle));
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = height;
	}
}

void HeightMap::Gradient(Point source, Point destination, Height fromHeight, Height toHeight)
{
	// Points are not used because greater value type is required for calculations below.
	long long gradientOffsetX = destination.GetX() - (long long)source.GetX();
	long long gradientOffsetY = destination.GetY() - (long long)source.GetY();

	Point gradientOffset(gradientOffsetX, gradientOffsetY);

	// Width of the gradient strip.
	unsigned long long maxDistance = source.GetDistanceTo(destination);//  sqrt((double)(abs(gradientOffsetX) * abs(gradientOffsetX) + abs(gradientOffsetY) * abs(gradientOffsetY)));

	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		long long currentOffsetX = x - (long long)source.GetX();
		long long currentOffsetY = y - (long long)source.GetX();

		// Get the point on the gradient vector (vector going through both source and destination point) to which is the current point closest.
		Point cross(
			Coordinate((gradientOffsetX * (gradientOffsetX * currentOffsetX + gradientOffsetY * currentOffsetY)) / (gradientOffsetX * gradientOffsetX + gradientOffsetY * gradientOffsetY)),
			Coordinate((gradientOffsetY * (gradientOffsetX * currentOffsetX + gradientOffsetY * currentOffsetY)) / (gradientOffsetX * gradientOffsetX + gradientOffsetY * gradientOffsetY)));

		// Calculate the distance from the "from" point to the intersection with gradient vector.
		double distance = double(cross.GetDistanceFromOrigin());

		// Distance from  the intersection point to the destination point.
		double reverseDistance = double(cross.GetDistanceTo(gradientOffset)); // TODO: Subtract from total distance?
			
		// Apply it to the array data.
		if (distance <= maxDistance && reverseDistance <= maxDistance) {
			// TODO: lerp uses only coordinate, not long long
			(*this)(x, y) = Lerp(0, maxDistance, fromHeight, toHeight, distance);
		}
		else if (reverseDistance < distance) {
			(*this)(x, y) = toHeight;
		}
		else {
			(*this)(x, y) = fromHeight;
		}
	}
}

void HeightMap::Intersect(HeightMap* other)
{
	Rectangle intersection = Rectangle::Intersect(this->rectangle, other->rectangle);
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(intersection);

	Point offset = this->rectangle.GetPosition() - intersection.GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = min((*other)(x + offset.GetX(), y + offset.GetY()), (*this)(x, y));
	}
}

void HeightMap::Invert()
{
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = -(*this)(x, y);
	}
}

void HeightMap::Move(Point offset)
{
    // TODO: range check
	this->rectangle += offset;
}

void HeightMap::Multiply(Height factor)
{
	double actualFactor = factor / (double)HEIGHT_MAX;

	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = Height((*this)(x, y) * factor);
	}
}

void HeightMap::MultiplyMap(HeightMap* factor)
{
	Rectangle intersection = Rectangle::Intersect(this->rectangle, factor->rectangle);
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(intersection);

	Point offset = this->rectangle.GetPosition() - intersection.GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = Height((*this)(x, y) * ((*factor)(x + offset.GetX(), y + offset.GetY()) / (double)HEIGHT_MAX));
	}
}

void HeightMap::Projection(HeightProfile* profile, Direction direction)
{
	Rectangle profileRect;
	if (direction == DIRECTION_HORIZONTAL)
	{
		profileRect = Rectangle(Point(COORDINATE_MIN, profile->GetInterval().GetStart()), Size2D(SIZE1D_MAX, profile->GetInterval().GetLength()));
	}
	else if (direction == DIRECTION_VERTICAL)
	{
		profileRect = Rectangle(Point(profile->GetInterval().GetStart(), COORDINATE_MIN), Size2D(profile->GetInterval().GetLength(), SIZE1D_MAX));
	}
	else throw InternalErrorException(GG_STR("Invalid direction."));

	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(Rectangle::Intersect(this->rectangle, profileRect));


	if (direction == DIRECTION_HORIZONTAL)
	{
		/*Rectangle operationRect = this->GetPhysicalRectangle(Rectangle(
			Point(this->GetOriginX(), profile->GetStart()),
			Size2D(this->GetWidth(), profile->GetLength())));*/

		FOR_EACH_IN_RECT(x, y, operationRect)
		{
			(*this)(x, y) = (*profile)(y);
		}
	}
	else if (direction == DIRECTION_VERTICAL)
	{
		/*Rectangle operationRect = this->GetPhysicalRectangle(Rectangle(
			Point(profile->GetStart(), this->GetOriginY()),
			Size2D(profile->GetLength(), this->GetHeight())));*/

		FOR_EACH_IN_RECT(x, y, operationRect)
		{
			(*this)(x, y) = (*profile)(x);
		}
	}
	else throw InternalErrorException(GG_STR("Invalid direction."));
}

void HeightMap::RadialGradient(Point point, Size1D radius, Height fromHeight, Height toHeight)
{
	Point physicalCenter = this->GetPhysicalPoint(point);
	Size1D scaledRadius = this->GetScaledSize(radius);

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

void HeightMap::Pattern(HeightMap* pattern, Rectangle repeatRectangle)
{
	if (repeatRectangle.GetSize().GetTotalLength() == 0)
	{
		return;
	}

	Rectangle physicalRepeatRect = Rectangle::Intersect(pattern->GetPhysicalRectangle(repeatRectangle), pattern->GetPhysicalRectangleUnscaled(pattern->GetRectangle()));
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		Point patternPoint = Point((x + this->GetOriginX()) % physicalRepeatRect.GetSize().GetWidth(), (y + this->GetOriginY()) % physicalRepeatRect.GetSize().GetHeight());
		if (patternPoint.GetX() < 0) patternPoint += Point(physicalRepeatRect.GetSize().GetWidth(), 0);
		if (patternPoint.GetY() < 0) patternPoint += Point(0, physicalRepeatRect.GetSize().GetHeight());
		(*this)(x, y) = (*pattern)(patternPoint + physicalRepeatRect.GetPosition());
	}
}

void HeightMap::Rescale(Scale horizontalScale, Scale verticalScale)
{
	Rectangle newRectangle(Point(Coordinate(this->rectangle.GetPosition().GetX() * horizontalScale), Coordinate(this->rectangle.GetPosition().GetY() * verticalScale)), Size2D(Size1D(this->rectangle.GetSize().GetWidth() * horizontalScale), Size1D(this->rectangle.GetSize().GetHeight() * verticalScale)));

	// Allocate the new array.
	Height* newData = new Height[newRectangle.GetSize().GetTotalLength()];

	Rectangle operationRect = newRectangle - newRectangle.GetPosition();
	
	double actualHorizontalScale = (newRectangle.GetSize().GetWidth() - 1) / (double)(this->rectangle.GetSize().GetWidth() - 1);
	double actualVerticalScale = (newRectangle.GetSize().GetHeight() - 1) / (double)(this->rectangle.GetSize().GetHeight() - 1);
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		newData[x + newRectangle.GetSize().GetWidth() * y] = (*this)(double(x / actualHorizontalScale), double(y / actualVerticalScale));
	}

	// Relink and delete the original array data
	delete[] this->heightData;
	this->heightData = newData;
	this->rectangle = newRectangle;
}

void HeightMap::Shift(HeightProfile* profile, Size1D maximumDistance, Direction direction)
{	
	Rectangle profileRect;
	if (direction == DIRECTION_HORIZONTAL)
	{
		profileRect = Rectangle(Point(COORDINATE_MIN, profile->GetInterval().GetStart()), Size2D(SIZE1D_MAX, profile->GetInterval().GetLength()));
	}
	else if (direction == DIRECTION_VERTICAL)
	{
		profileRect = Rectangle(Point(profile->GetInterval().GetStart(), COORDINATE_MIN), Size2D(profile->GetInterval().GetLength(), SIZE1D_MAX));
	}
	else throw InternalErrorException(GG_STR("Invalid direction."));

	Rectangle contraction = Rectangle::Contract(this->rectangle, maximumDistance, direction);
	Rectangle logicalRect = Rectangle::Intersect(contraction, profileRect);
	Rectangle physicalRect = this->GetPhysicalRectangleUnscaled(logicalRect);

	Coordinate profileOffset;
	if (direction == DIRECTION_HORIZONTAL)
	{
		profileOffset = logicalRect.GetPosition().GetY() - profile->GetStart();
	}
	else if (direction == DIRECTION_VERTICAL)
	{
		profileOffset = logicalRect.GetPosition().GetX() - profile->GetStart();
	}

	// Allocate the new array.
	Height* newData = new Height[this->rectangle.GetSize().GetTotalLength()];
	memset(newData, 0, sizeof(Height)* this->rectangle.GetSize().GetTotalLength());

	double factor = maximumDistance / double(HEIGHT_MAX);

	if (direction == DIRECTION_HORIZONTAL)
	{
		FOR_EACH_IN_RECT(x, y, physicalRect)
		{			
			newData[x + this->GetWidth() * y] = (*this)(x - (*profile)(y + profileOffset) * factor, double(y));
		}
	}
	else if (direction == DIRECTION_VERTICAL)
	{
		FOR_EACH_IN_RECT(x, y, physicalRect)
		{

			newData[x + this->GetWidth() * y] = (*this)(double(x), y - (*profile)(x + profileOffset)* factor);
		}
	}

	delete[] this->heightData;
	this->heightData = newData;
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

void HeightMap::Unify(HeightMap* other)
{
	Rectangle intersection = Rectangle::Intersect(this->rectangle, other->rectangle);
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(intersection);

	Point offset = this->rectangle.GetPosition() - intersection.GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = max((*other)(x + offset.GetX(), y + offset.GetY()), (*this)(x, y));
	}
}