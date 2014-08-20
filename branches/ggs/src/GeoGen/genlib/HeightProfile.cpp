#include "HeightProfile.hpp"
#include "../random/RandomSequence2D.hpp"
#include "../ApiUsageException.hpp"
#include "NoiseLayer.hpp"

using namespace geogen;
using namespace genlib;
using namespace random;
using namespace std;

#define FOR_EACH_IN_INTERVAL(x, interval) \
	for (Coordinate x = interval.GetStart(); x < interval.GetEnd(); x++)

HeightProfile::HeightProfile(Interval interval, Scale scale)
:interval(interval), scale(scale)
{
	this->heightData = new Height[interval.GetLength()];
	memset(this->heightData, 0, interval.GetLength());
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
		(*this)(x) += addend;
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
		Height maskHeight = (*mask)(x + maskOffset);
		(*this)(x) += addend * Height(maskHeight / (double)HEIGHT_MAX);
	}
}

void HeightProfile::AddMap(HeightProfile* addend)
{
	Interval intersection = Interval::Intersect(this->interval, addend->interval);
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(intersection);

	Coordinate offset = this->interval.GetStart() - intersection.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) += (*addend)(x + offset);
	}
}

void HeightProfile::AddMapMasked(HeightProfile* addend, HeightProfile* mask)
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
		Height maskHeight = (*mask)(x + maskOffset);
		(*this)(x) += addendHeight * Height(maskHeight / (double)HEIGHT_MAX);
	}
}

void HeightProfile::Blur(Size1D radius)
{

	// Allocate the new array.
	Height* newData = new Height[this->interval.GetLength()];

	Size1D scaledRadius = this->GetScaledSize(radius);

	// Prefill the window with value of the left edge + n leftmost values (where n is kernel size).
	Size1D windowSize = scaledRadius * 2 + 1;
	long long windowValue = (*this)(0) * scaledRadius;

	for (unsigned x = 0; x < scaledRadius; x++) {
		windowValue += (*this)(Coordinate(x));
	}

	/* In every step shift the window one tile to the right  (= subtract its leftmost cell and add
	value of rightmost + 1). i represents position of the central cell of the window. */
	for (unsigned x = 0; x < this->GetLength(); x++) {
		// If the window is approaching right border, use the rightmost value as fill.
		if (x < scaledRadius) {
			windowValue += (*this)(Coordinate(x + scaledRadius)) - (*this)(0);
		}
		else if (x + scaledRadius < this->GetLength()) {
			windowValue += (*this)(Coordinate(x + scaledRadius)) - (*this)(Coordinate(x - scaledRadius));
		}
		else {
			windowValue += (*this)(Coordinate(this->GetLength() - 1)) - (*this)(Coordinate(x - scaledRadius));
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
	Coordinate maskOffset = mask->GetInterval().GetStart() - this->interval.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		Height thisHeight = (*this)(x);
		Height otherHeight = (*other)(x + otherOffset);
		Height maskHeight = (*mask)(x + maskOffset);
		double factor = maskHeight / (double)HEIGHT_MAX;
		(*this)(x) += Height(thisHeight * factor) + Height(otherHeight * (1 - factor));
	}
}

void HeightProfile::ConvexityMap(Size1D radius)
{
	HeightProfile unsmoothed(*this);

	/* Convexity map is a difference between the current map and its smoothed variant. Smoothing erases any terrain features
	that peak upwards (are convex) or bulge downwards (are concave). */
	this->Blur(radius);
	this->Invert();
	this->AddMap(&unsmoothed);
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

void HeightProfile::Gradient(Coordinate source, Coordinate destination, Height fromHeight, Height toHeight)
{
	Coordinate start = min(source, destination);
	Coordinate end = max(source, destination);
	Height startHeight = source == start ? fromHeight : toHeight;
	Height endHeight = source == end ? fromHeight : toHeight;

	Size1D gradientLength = abs(destination - source);
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		if (x < start)
		{
			(*this)(x) = fromHeight;
		}
		else if (x > end)
		{
			(*this)(x) = toHeight;
		}
		else
		{
			(*this)(x) = Lerp(start, end, fromHeight, toHeight, start + ((x - start) / double(gradientLength)));
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
	this->interval += offset;
}

void HeightProfile::Multiply(Height factor)
{
	double actualFactor = factor / (double)HEIGHT_MAX;

	Interval operationInterval = this->GetPhysicalIntervalUnscaled(this->interval);

	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = Height((*this)(x) * factor);
	}
}

void HeightProfile::MultiplyMap(HeightProfile* factor)
{
	Interval intersection = Interval::Intersect(this->interval, factor->interval);
	Interval operationInterval = this->GetPhysicalIntervalUnscaled(intersection);

	Coordinate offset = this->interval.GetStart() - intersection.GetStart();
	FOR_EACH_IN_INTERVAL(x, operationInterval)
	{
		(*this)(x) = Height((*this)(x) * ((*factor)(x + offset) / (double)HEIGHT_MAX));
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

void HeightProfile::Noise(std::vector<NoiseLayer> layers, RandomSeed seed)
{
	
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