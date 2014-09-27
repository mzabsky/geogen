#include <vector>
#include <algorithm>

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
		(*this)(x, y) = AddHeights((*this)(x, y), addend);
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
		Height maskHeight = max((*mask)(x + maskOffset.GetX(), y + maskOffset.GetY()), Height(0));
		(*this)(x, y) = AddHeights((*this)(x, y), addend * Height(maskHeight / (double)HEIGHT_MAX));
	}
}

void HeightMap::AddMap(HeightMap* addend)
{
	Rectangle intersection = Rectangle::Intersect(this->rectangle, addend->rectangle);
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(intersection);

	Point offset = this->rectangle.GetPosition() - intersection.GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRect) 
	{
		(*this)(x, y) = AddHeights((*this)(x, y), (*addend)(x + offset.GetX(), y + offset.GetY()));
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
		Height maskHeight = max((*mask)(x + maskOffset.GetX(), y + maskOffset.GetY()), Height(0));
		(*this)(x, y) = AddHeights((*this)(x, y), addendHeight * Height(maskHeight / (double)HEIGHT_MAX));
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
		Height maskHeight = max((*mask)(x + maskOffset.GetX(), y + maskOffset.GetY()), Height(0));
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

void HeightMap::Crop(Rectangle fillRectangle, Height height)
{
	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		if (!rectangle.Contains(this->GetLogicalPoint(Point(x, y))))
		{
			(*this)(x, y) = height;
		}
	}
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

void HeightMap::DistanceMap(Size1D distance, Direction direction)
{

}

void HeightMap::DistanceMap(Size1D distance)
{
	this->DistanceMap(distance, DIRECTION_HORIZONTAL);
	this->DistanceMap(distance, DIRECTION_VERTICAL);
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
		long long currentOffsetY = y - (long long)source.GetY();

		// Get the point on the gradient vector (vector going through both source and destination point) to which is the current point closest.
		double crossX = (gradientOffsetX * (gradientOffsetX * currentOffsetX + gradientOffsetY * currentOffsetY)) / double(gradientOffsetX * gradientOffsetX + gradientOffsetY * gradientOffsetY);
		double crossY = (gradientOffsetY * (gradientOffsetX * currentOffsetX + gradientOffsetY * currentOffsetY)) / double(gradientOffsetX * gradientOffsetX + gradientOffsetY * gradientOffsetY);

		// Calculate the distance from the "from" point to the intersection with gradient vector.
		double distance = sqrt(crossX * crossX + crossY * crossY);

		// Distance from  the intersection point to the destination point.
		double reverseDistance = sqrt((crossX - gradientOffsetX) * (crossX - gradientOffsetX) + (crossY - gradientOffsetY) * (crossY - gradientOffsetY));
			
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
	this->rectangle += offset * this->scale;
}

void HeightMap::Multiply(Height factor)
{
	double actualFactor = factor / (double)HEIGHT_MAX;

	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		(*this)(x, y) = Height((*this)(x, y) * actualFactor);
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
	double scaledRadius = this->GetScaledSize(radius);

	Rectangle physicalRect = this->GetPhysicalRectangleUnscaled(this->rectangle);
	FOR_EACH_IN_RECT(x, y, physicalRect)
	{
		double distance = physicalCenter.GetDistanceTo(Point(x, y));

		if (distance > scaledRadius)
		{
			(*this)(x, y) = toHeight;
		}
		else
		{
			(*this)(x, y) = fromHeight + (Height)(((long long)toHeight - (long long)fromHeight) * distance / scaledRadius);
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

void HeightMap::Resize(Rectangle rectangle, Height height)
{
	HeightMap old(*this);

	delete[] this->heightData;

	this->heightData = new Height[rectangle.GetSize().GetTotalLength()];
	this->rectangle = rectangle;

	Rectangle operationRectangle = this->GetPhysicalRectangleUnscaled(this->rectangle);
	Point offset = this->rectangle.GetPosition() - old.GetRectangle().GetPosition();
	FOR_EACH_IN_RECT(x, y, operationRectangle)
	{
		if (old.GetRectangle().Contains(this->GetLogicalPoint(Point(x, y))))
		{
			(*this)(x, y) = old(Point(x, y) + offset);
		}
		else
		{
			(*this)(x, y) = height;
		}
	}
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

void HeightMap::Noise(NoiseLayers const& layers, RandomSeed seed)
{
	this->FillRectangle(RECTANGLE_MAX, 0);

	unsigned i = 0;
	for (NoiseLayers::const_iterator it = layers.begin(); it != layers.end(); it++)
	{		
		this->NoiseLayer(it->first, it->second, seed, i);
		i++;
	}
}

void HeightMap::NoiseLayer(Size1D waveLength, Height amplitude, RandomSeed seed, unsigned seedStep)
{
	RandomSequence2D randomSequence(seed);
	for (unsigned i = 0; i < seedStep; i++)
	{
		randomSequence.Advance();
	}

	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(this->rectangle);

	Size1D physicalWaveLength = this->GetScaledSize(waveLength);

	if (physicalWaveLength == 1)
	{
		// For wave length 1 no interpolation is necessary.

		FOR_EACH_IN_RECT(x, y, operationRect)
		{
			Point logicalPoint = this->GetLogicalPoint(Point(x, y));

			(*this)(x, y) += (Height)randomSequence.GetInt(logicalPoint, -amplitude, +amplitude);
		}
	}
	else if (physicalWaveLength > 1)
	{
		FOR_EACH_IN_RECT(x, y, operationRect)
		{
			Point logicalPoint = this->GetLogicalPoint(Point(x, y));

			Coordinate coordinateX1 = PreviousMultipleOfInclusive(logicalPoint.GetX(), waveLength);
			Coordinate coordinateX0 = PreviousMultipleOfExclusive(coordinateX1, waveLength);
			Coordinate coordinateX2 = NextMultipleOfExclusive(logicalPoint.GetX(), waveLength);
			Coordinate coordinateX3 = NextMultipleOfExclusive(coordinateX2, waveLength);

			Coordinate coordinateY1 = PreviousMultipleOfInclusive(logicalPoint.GetY(), waveLength);
			Coordinate coordinateY0 = PreviousMultipleOfExclusive(coordinateY1, waveLength);
			Coordinate coordinateY2 = NextMultipleOfExclusive(logicalPoint.GetY(), waveLength);
			Coordinate coordinateY3 = NextMultipleOfExclusive(coordinateY2, waveLength);

			double height00 = (Height)randomSequence.GetInt(Point(coordinateX0, coordinateY0), -amplitude, +amplitude);
			double height10 = (Height)randomSequence.GetInt(Point(coordinateX1, coordinateY0), -amplitude, +amplitude);
			double height20 = (Height)randomSequence.GetInt(Point(coordinateX2, coordinateY0), -amplitude, +amplitude);
			double height30 = (Height)randomSequence.GetInt(Point(coordinateX3, coordinateY0), -amplitude, +amplitude);
			double height01 = (Height)randomSequence.GetInt(Point(coordinateX0, coordinateY1), -amplitude, +amplitude);
			double height11 = (Height)randomSequence.GetInt(Point(coordinateX1, coordinateY1), -amplitude, +amplitude);
			double height21 = (Height)randomSequence.GetInt(Point(coordinateX2, coordinateY1), -amplitude, +amplitude);
			double height31 = (Height)randomSequence.GetInt(Point(coordinateX3, coordinateY1), -amplitude, +amplitude);
			double height02 = (Height)randomSequence.GetInt(Point(coordinateX0, coordinateY2), -amplitude, +amplitude);
			double height12 = (Height)randomSequence.GetInt(Point(coordinateX1, coordinateY2), -amplitude, +amplitude);
			double height22 = (Height)randomSequence.GetInt(Point(coordinateX2, coordinateY2), -amplitude, +amplitude);
			double height32 = (Height)randomSequence.GetInt(Point(coordinateX3, coordinateY2), -amplitude, +amplitude);
			double height03 = (Height)randomSequence.GetInt(Point(coordinateX0, coordinateY3), -amplitude, +amplitude);
			double height13 = (Height)randomSequence.GetInt(Point(coordinateX1, coordinateY3), -amplitude, +amplitude);
			double height23 = (Height)randomSequence.GetInt(Point(coordinateX2, coordinateY3), -amplitude, +amplitude);
			double height33 = (Height)randomSequence.GetInt(Point(coordinateX3, coordinateY3), -amplitude, +amplitude);

			// Prepare coefficients for the bicubic polynomial.
			// TODO: These need to be recalculated only when the grid cell changes
			double a00 = height11;
			double a01 = -.5*height10 + .5*height12;
			double a02 = height10 - 2.5*height11 + 2 * height12 - .5*height13;
			double a03 = -.5*height10 + 1.5*height11 - 1.5*height12 + .5*height13;
			double a10 = -.5*height01 + .5*height21;
			double a11 = .25*height00 - .25*height02 - .25*height20 + .25*height22;
			double a12 = -.5*height00 + 1.25*height01 - height02 + .25*height03 + .5*height20 - 1.25*height21 + height22 - .25*height23;
			double a13 = .25*height00 - .75*height01 + .75*height02 - .25*height03 - .25*height20 + .75*height21 - .75*height22 + .25*height23;
			double a20 = height01 - 2.5*height11 + 2 * height21 - .5*height31;
			double a21 = -.5*height00 + .5*height02 + 1.25*height10 - 1.25*height12 - height20 + height22 + .25*height30 - .25*height32;
			double a22 = height00 - 2.5*height01 + 2 * height02 - .5*height03 - 2.5*height10 + 6.25*height11 - 5 * height12 + 1.25*height13 + 2 * height20 - 5 * height21 + 4 * height22 - height23 - .5*height30 + 1.25*height31 - height32 + .25*height33;
			double a23 = -.5*height00 + 1.5*height01 - 1.5*height02 + .5*height03 + 1.25*height10 - 3.75*height11 + 3.75*height12 - 1.25*height13 - height20 + 3 * height21 - 3 * height22 + height23 + .25*height30 - .75*height31 + .75*height32 - .25*height33;
			double a30 = -.5*height01 + 1.5*height11 - 1.5*height21 + .5*height31;
			double a31 = .25*height00 - .25*height02 - .75*height10 + .75*height12 + .75*height20 - .75*height22 - .25*height30 + .25*height32;
			double a32 = -.5*height00 + 1.25*height01 - height02 + .25*height03 + 1.5*height10 - 3.75*height11 + 3 * height12 - .75*height13 - 1.5*height20 + 3.75*height21 - 3 * height22 + .75*height23 + .5*height30 - 1.25*height31 + height32 - .25*height33;
			double a33 = .25*height00 - .75*height01 + .75*height02 - .25*height03 - .75*height10 + 2.25*height11 - 2.25*height12 + .75*height13 + .75*height20 - 2.25*height21 + 2.25*height22 - .75*height23 - .25*height30 + .75*height31 - .75*height32 + .25*height33;

			double remainderX = (logicalPoint.GetX() - coordinateX1) / (double)waveLength;
			double remainderY = (logicalPoint.GetY() - coordinateY1) / (double)waveLength;

			// Calculate value of the bicubic polynomial.
			double remainderX2 = remainderX * remainderX;
			double remainderX3 = remainderX2 * remainderX;
			double remainderY2 = remainderY * remainderY;
			double remainderY3 = remainderY2 * remainderY;

			double result = (a00 + a01 * remainderY + a02 * remainderY2 + a03 * remainderY3) +
				(a10 + a11 * remainderY + a12 * remainderY2 + a13 * remainderY3) * remainderX +
				(a20 + a21 * remainderY + a22 * remainderY2 + a23 * remainderY3) * remainderX2 +
				(a30 + a31 * remainderY + a32 * remainderY2 + a33 * remainderY3) * remainderX3;

			(*this)(x, y) = (Height)std::max(min(result + (*this)(x, y), (double)HEIGHT_MAX), (double)HEIGHT_MIN);
		}

		// Don't bother with too short wave lengths
	}
}

void HeightMap::Transform(TransformationMatrix const& matrix, Rectangle transformedRectangle)
{
	TransformationMatrix invertedMatrix = TransformationMatrix::Inverse(matrix);

	auto_ptr<HeightMap> oldThis = auto_ptr<HeightMap>(new HeightMap(*this));

	this->rectangle = transformedRectangle;
	delete[] heightData;
	this->heightData = new Height[transformedRectangle.GetSize().GetTotalLength()];

	this->FillRectangle(RECTANGLE_MAX, 0);

	Rectangle operationRect = this->GetPhysicalRectangleUnscaled(transformedRectangle);
	FOR_EACH_IN_RECT(x, y, operationRect)
	{
		Point logicalPoint = this->GetLogicalPoint(Point(x, y));
		double transformedX = invertedMatrix.GetTransformedX(logicalPoint);
		double transformedY = invertedMatrix.GetTransformedY(logicalPoint);
		double sourceX = oldThis->GetPhysicalCoordinate(transformedX, DIRECTION_HORIZONTAL);
		double sourceY = oldThis->GetPhysicalCoordinate(transformedY, DIRECTION_VERTICAL);
		(*this)(x, y) = (*oldThis)(sourceX, sourceY);
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