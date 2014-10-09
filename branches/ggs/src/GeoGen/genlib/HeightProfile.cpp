#include <algorithm>

#include "HeightProfile.hpp"
#include "../random/RandomSequence2D.hpp"
#include "../ApiUsageException.hpp"
#include "HeightMap.hpp"

using namespace geogen;
using namespace genlib;
using namespace random;
using namespace std;

#define FOR_EACH_IN_INTERVAL(x, interval) \
	for (Coordinate x = interval.GetStart(); x < interval.GetEnd(); x++)

HeightProfile::HeightProfile(Interval interval, Height height, Scale scale)
:interval(interval), scale(scale)
{
	this->heightData = new Height[interval.GetLength()];

	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = height;
	}
}

HeightProfile::HeightProfile(HeightProfile const& other)
{
	this->interval = other.interval;
	this->scale = other.scale;

	this->heightData = new Height[this->interval.GetLength()];
	memcpy(this->heightData, other.heightData, sizeof(Height) * this->interval.GetLength());
}

HeightProfile::HeightProfile(HeightProfile const& other, Interval cutoutInterval)
{
	this->interval = cutoutInterval;
	this->heightData = new Height[cutoutInterval.GetLength()];
	this->scale = other.scale;

	Interval physicalInterval = this->GetPhysicalIntervalUnscaled(cutoutInterval);

	// Fill all pixels with 0, because the cutout interval might have only partially overlapped with the original interval.
	FOR_EACH_IN_INTERVAL(x, physicalInterval)
	{
		(*this)(x) = 0;
	}

	Interval intersection = this->GetPhysicalIntervalUnscaled(Interval::Intersect(other.interval, cutoutInterval));

	Coordinate offset = cutoutInterval.GetStart() - other.interval.GetStart();
	FOR_EACH_IN_INTERVAL(x, intersection)
	{
		(*this)(x) = other(x + offset);
	}
}

HeightProfile& HeightProfile::operator=(HeightProfile& other)
{
	this->interval = other.interval;
	this->scale = other.scale;

	delete[] this->heightData;

	this->heightData = new Height[this->interval.GetLength()];
	memcpy(this->heightData, other.heightData, sizeof(Height) * this->interval.GetLength());

	return *this;
}

HeightProfile::~HeightProfile()
{
	delete[] this->heightData;
}

void HeightProfile::Abs()
{
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = (*this)(x) > 0 ? (*this)(x) : -(*this)(x);
	}
}

void HeightProfile::Add(Height addend)
{
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = AddHeights((*this)(x), addend);
	}
}

void HeightProfile::AddMasked(Height addend, HeightProfile* mask)
{
	if (!mask->interval.Contains(this->interval))
	{
		throw ApiUsageException(GG_STR("Mask is too small."));
	}

	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	Coordinate maskOffset = mask->GetInterval().GetStart() - this->interval.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		double maskHeight = max((*mask)(x + maskOffset), Height(0));
		(*this)(x) = AddHeights((*this)(x), Height(addend * maskHeight / (double)HEIGHT_MAX));
	}
}

void HeightProfile::AddProfile(HeightProfile* addend)
{
	Interval intersection = Interval::Intersect(this->interval, addend->interval);
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(intersection);

	Coordinate offset = this->interval.GetStart() - intersection.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = AddHeights((*this)(x), (*addend)(x + offset));
	}
}

void HeightProfile::AddProfileMasked(HeightProfile* addend, HeightProfile* mask)
{
	Interval intersection = Interval::Intersect(this->interval, addend->interval);

	if (!mask->interval.Contains(intersection))
	{
		throw ApiUsageException(GG_STR("Mask is too small."));
	}

	Interval operationInterval = this->GetPhysicalIntervalUnscaled(intersection);

	Coordinate addendOffset = this->interval.GetStart() - intersection.GetStart();
	Coordinate maskOffset = mask->GetInterval().GetStart() - this->interval.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		Height addendHeight = (*addend)(x + addendOffset);
		double maskHeight = max((*mask)(x + maskOffset), Height(0));
		(*this)(x) = AddHeights((*this)(x), Height(addendHeight * maskHeight / (double)HEIGHT_MAX));
	}
}

void HeightProfile::Blur(Size1D radius)
{

	// Allocate the new array.
	Height* newData = new Height[this->interval.GetLength()];

	Size1D scaledRadius = this->GetScaledSize(radius);

	// Prefill the window with value of the left edge + n leftmost values (where n is kernel size).
	Size1D windowSize = scaledRadius * 2 + 1;
	long long windowValue = (long long)(*this)(0) * scaledRadius;

	for (unsigned x = 0; x < scaledRadius; x++) {
		windowValue += (long long)(*this)(Coordinate(x));
	}

	/* In every step shift the window one tile to the right  (= subtract its leftmost cell and add
	value of rightmost + 1). i represents position of the central cell of the window. */
	for (unsigned x = 0; x < this->GetLength(); x++) {
		// If the window is approaching right border, use the rightmost value as fill.
		if (x < scaledRadius) {
			windowValue += (long long)(*this)(Coordinate(x + scaledRadius)) - (long long)(*this)(0);
		}
		else if (x + scaledRadius < this->GetLength()) {
			windowValue += (long long)(*this)(Coordinate(x + scaledRadius)) - (long long)(*this)(Coordinate(x - scaledRadius));
		}
		else {
			windowValue += (long long)(*this)(Coordinate(this->GetLength() - 1)) - (long long)(*this)(Coordinate(x - scaledRadius));
		}

		// Set the value of current tile to arithmetic average of window tiles.
		newData[x] = Height(windowValue / windowSize);
	}

	// Relink and delete the original array data
	delete[] this->heightData;
	this->heightData = newData;
}

void HeightProfile::ClampHeights(Height min, Height max)
{
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = std::min(max, std::max(min, (*this)(x)));
	}
}

void HeightProfile::Combine(HeightProfile* other, HeightProfile* mask)
{
	Interval intersection = Interval::Intersect(this->interval, other->interval);

	if (!mask->interval.Contains(intersection))
	{
		throw ApiUsageException(GG_STR("Mask is too small."));
	}

	Interval operationInterval = this->GetPhysicalIntervalUnscaled(intersection);

	Coordinate otherOffset = this->interval.GetStart() - intersection.GetStart();
	Coordinate maskOffset = this->interval.GetStart() - mask->GetInterval().GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		Height thisHeight = (*this)(x);
		Height otherHeight = (*other)(x + otherOffset);
		Height maskHeight = max((*mask)(x + maskOffset), Height(0));
		double factor = maskHeight / (double)HEIGHT_MAX;
		(*this)(x) += Height(thisHeight * factor) + Height(otherHeight * (1 - factor));
	}
}

void HeightProfile::Crop(Interval interval, Height height)
{
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		if (!interval.Contains(this->GetLogicalCoordinate(x)))
		{
			(*this)(x) = height;
		}
	}
}

void HeightProfile::CropHeights(Height min, Height max, Height replace)
{
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		if ((*this)(x) > max || (*this)(x) < min)
		{
			(*this)(x) = replace;
		}
	}
}

void HeightProfile::FillInterval(Interval fillInterval, Height height)
{
	Interval operationInterval = Interval::Intersect(this->GetPhysicalIntervalUnscaled(this->interval), this->GetPhysicalInterval(fillInterval));
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = height;
	}
}

void HeightProfile::Flip()
{
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	HeightProfile old(*this);

	this->interval = Interval(-this->interval.GetStart() - this->interval.GetLength(), this->interval.GetLength());
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = old(Coordinate(this->interval.GetLength() - x - 1));
	}
}

void HeightProfile::FromArray(std::map<Coordinate, Height> const& heights)
{
	if (heights.size() == 0)
	{
		return;
	}
	else if (heights.size() == 1)
	{
		this->FillInterval(INTERVAL_MAX, heights.begin()->second);
	}

	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	if (heights.begin()->first > this->GetStart())
	{
		this->FillInterval(Interval(this->GetStart(), heights.begin()->first - this->GetStart()), heights.begin()->second);
	}

	std::map<Coordinate, Height>::const_iterator last = --heights.end();
	if (last->first < this->interval.GetEnd())
	{
		this->FillInterval(Interval(last->first, this->interval.GetEnd() - last->first), last->second);
	}
	
	Coordinate lastCoordinate = heights.begin()->first;
	Height lastHeight = heights.begin()->second;
	for (std::map<Coordinate, Height>::const_iterator it = ++heights.begin(); it != heights.end(); it++)
	{
		this->Gradient(lastCoordinate, it->first, lastHeight, it->second, false);
		lastCoordinate = it->first;
		lastHeight = it->second;
	}
}

void HeightProfile::Gradient(Coordinate source, Coordinate destination, Height fromHeight, Height toHeight, bool fillOutside)
{
	Coordinate start = this->GetPhysicalCoordinate(min(source, destination));
	Coordinate end = this->GetPhysicalCoordinate(max(source, destination));
	Height startHeight = source == start ? fromHeight : toHeight;
	Height endHeight = source == end ? fromHeight : toHeight;

	Size1D gradientLength = abs(destination - source);
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		Coordinate logicalX = this->GetLogicalCoordinate(x);

		if (x >= start && x <= end)
		{
			Height lerp = Lerp(start, end, fromHeight, toHeight, x/*start + ((x - start) / double(gradientLength))*/);
			(*this)(x) = Lerp(start, end, fromHeight, toHeight, x/*start + ((x - start) / double(gradientLength))*/);
		}
		else if (fillOutside && x < start)
		{
			(*this)(x) = fromHeight;
		}
		else if (fillOutside && x > end)
		{
			(*this)(x) = toHeight;
		}
	}
}

void HeightProfile::Intersect(HeightProfile* other)
{
	Interval intersection = Interval::Intersect(this->interval, other->interval);
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(intersection);

	Coordinate offset = this->interval.GetStart() - intersection.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = min((*other)(x + offset), (*this)(x));
	}
}

void HeightProfile::Invert()
{
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = -(*this)(x);
	}
}

void HeightProfile::Move(Coordinate offset)
{
	// TODO: range check
	this->interval += Coordinate(offset * this->scale);
}

void HeightProfile::Multiply(double factor)
{
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = (Height)min((double)HEIGHT_MAX, max((double)HEIGHT_MIN, (*this)(x) * factor));
	}
}

void HeightProfile::MultiplyProfile(HeightProfile* factor)
{
	Interval intersection = Interval::Intersect(this->interval, factor->interval);
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(intersection);

	Coordinate offset = this->interval.GetStart() - intersection.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = Height((*this)(x) * ((*factor)(x + offset) / (double)HEIGHT_MAX));
	}
}

void HeightProfile::Noise(NoiseLayers const& layers, RandomSeed seed)
{
	this->FillInterval(INTERVAL_MAX, 0);

	unsigned i = 0;
	for (NoiseLayers::const_iterator it = layers.begin(); it != layers.end(); it++)
	{
		this->NoiseLayer(it->first, it->second, seed, i);
		i++;
	}
}

void HeightProfile::NoiseLayer(Size1D waveLength, Height amplitude, random::RandomSeed seed, unsigned seedStep)
{
	RandomSequence2D randomSequence(seed);
	for (unsigned i = 0; i < seedStep; i++)
	{
		randomSequence.Advance();
	}

	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	Size1D physicalWaveLength = this->GetScaledSize(waveLength);

	if (physicalWaveLength == 1)
	{
		// For wave length 1 no interpolation is necessary.

		FOR_EACH_IN_INTERVAL(x, operationInterval)
		{
			Coordinate logicalCoordinate = this->GetLogicalCoordinate(x);

			(*this)(x) += (Height)randomSequence.GetInt(logicalCoordinate, -amplitude, +amplitude);
		}
	}
	else if (physicalWaveLength > 1)
	{
		FOR_EACH_IN_INTERVAL(x, operationInterval)
		{
			Coordinate logicalCoordinate = this->GetLogicalCoordinate(x);

			if (logicalCoordinate % waveLength == 0)
			{
				// Grid point has been hit, no interpolation is necessary
				(*this)(x) += (Height)randomSequence.GetInt(logicalCoordinate, -amplitude, +amplitude);
			}
			else
			{
				// Interpolate
					
				// We know the grid point is not being hit, use whichever of the Inclusive/Exclusive functions is faster.
				Coordinate coordinate1 = PreviousMultipleOfInclusive(logicalCoordinate, waveLength);
				Coordinate coordinate0 = PreviousMultipleOfExclusive(coordinate1, waveLength);
				Coordinate coordinate2 = NextMultipleOfExclusive(logicalCoordinate, waveLength);
				Coordinate coordinate3 = NextMultipleOfExclusive(coordinate2, waveLength);

				double height0 = randomSequence.GetInt(coordinate0, -amplitude, +amplitude);
				double height1 = randomSequence.GetInt(coordinate1, -amplitude, +amplitude);
				double height2 = randomSequence.GetInt(coordinate2, -amplitude, +amplitude);
				double height3 = randomSequence.GetInt(coordinate3, -amplitude, +amplitude);

				// Coefficients for the bicubic polynomial
				double a0 = (height3 - height2) - (height0 - height1);
				double a1 = (height0 - height1) - a0;
				double a2 = height2 - height0;
				double a3 = height1;

				double remainder = (logicalCoordinate - coordinate1) / (double)waveLength;

				// Calculate value of the cubic polynomial.
				double result = a0 * remainder * remainder * remainder + a1 * remainder * remainder + a2 * remainder + a3;

				(*this)(x) = (Height)std::max(min(result + (*this)(x), (double)HEIGHT_MAX), (double)HEIGHT_MIN);
			}
		}
	}

	// Don't bother with too short wave lengths
}

void HeightProfile::Pattern(HeightProfile* pattern, Interval repeatInterval)
{
	if (repeatInterval.GetLength())
	{
		return;
	}

	Interval physicalRepeatInterval = Interval::Intersect(pattern->GetPhysicalInterval(repeatInterval), pattern->GetPhysicalIntervalUnscaled(pattern->GetInterval()));
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		Coordinate patternIndex = (x + this->GetStart()) % physicalRepeatInterval.GetLength();
		if (patternIndex < 0) patternIndex += physicalRepeatInterval.GetLength();
		(*this)(x) = (*pattern)(patternIndex + physicalRepeatInterval.GetStart());
	}
}

void HeightProfile::Rescale(Scale scale)
{
	Interval newInterval(Coordinate(this->interval.GetStart() * scale), Size1D(this->interval.GetLength() * scale));

	// Allocate the new array.
	Height* newData = new Height[newInterval.GetLength()];

	Interval operationInterval = newInterval - newInterval.GetStart();

	double actualScale = (newInterval.GetLength() - 1) / (double)(this->interval.GetLength() - 1);
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		newData[x] = (*this)(double(x / actualScale));
	}

	// Relink and delete the original array data
	delete[] this->heightData;
	this->heightData = newData;
	this->interval = newInterval;
}

void HeightProfile::Resize(Interval interval, Height height)
{
	HeightProfile old(*this);

	delete[] this->heightData;

	this->heightData = new Height[interval.GetLength()];
	this->interval = interval;

	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);
	Coordinate offset = this->interval.GetStart() - old.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		if (old.GetInterval().Contains(this->GetLogicalCoordinate(x)))
		{
			(*this)(x) = old(x + offset);
		}
		else
		{
			(*this)(x) = height;
		}
	}
}

void HeightProfile::Slice(HeightMap* heightMap, Direction direction, Coordinate coordinate)
{
	this->FillInterval(INTERVAL_MAX, 0);

	Interval rectInterval = this->GetPhysicalInterval(Interval::FromRectangle(heightMap->GetRectangle(), direction));	

	Interval physicalInterval;
	if (direction == DIRECTION_HORIZONTAL)
	{
		if (coordinate < heightMap->GetOriginY() || coordinate >= heightMap->GetRectangle().GetEndingPoint().GetY())
		{
			physicalInterval = Interval();
		}
		else
		{
			physicalInterval = Interval::Intersect(rectInterval, this->GetPhysicalIntervalUnscaled(this->interval));
		}
	}
	else if (direction == DIRECTION_VERTICAL)
	{
		if (coordinate < heightMap->GetOriginX() || coordinate >= heightMap->GetRectangle().GetEndingPoint().GetX())
		{
			physicalInterval = Interval();
		}
		else
		{
			physicalInterval = Interval::Intersect(rectInterval, this->GetPhysicalIntervalUnscaled(this->interval));
		}
	}
	else throw ApiUsageException("Invalid direction.");
	

	if (direction == DIRECTION_HORIZONTAL)
	{
		Coordinate physicalCoordinate = heightMap->GetPhysicalPoint(Point(0, coordinate)).GetY();

		Coordinate offset = heightMap->GetOriginX() - this->GetStart();
		FOR_EACH_IN_INTERVAL(x, physicalInterval)
		{
			(*this)(x) = (*heightMap)(x + offset, physicalCoordinate);
		}
	}
	else if (direction == DIRECTION_VERTICAL)
	{
		Coordinate physicalCoordinate = heightMap->GetPhysicalPoint(Point(coordinate, 0)).GetX();

		Coordinate offset = this->GetStart() - heightMap->GetOriginY();
		FOR_EACH_IN_INTERVAL(x, physicalInterval)
		{
			(*this)(x) = (*heightMap)(physicalCoordinate, x + offset);
		}
	}
}

void HeightProfile::Unify(HeightProfile* other)
{
	Interval intersection = Interval::Intersect(this->interval, other->interval);
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(intersection);

	Coordinate offset = this->interval.GetStart() - intersection.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = max((*other)(x + offset), (*this)(x));
	}
}