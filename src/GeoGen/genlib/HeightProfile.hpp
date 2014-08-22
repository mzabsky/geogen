#pragma once

#include <vector>
#include <map>

#include "../Interval.hpp"
#include "../Number.hpp"
#include "DataObject.hpp"
#include "NoiseLayer.hpp"
#include "../random/RandomSeed.hpp"

namespace geogen
{
	namespace genlib
	{
		class HeightProfile : public DataObject
		{
		private:
			Interval interval;
			Height* heightData;
			Scale scale;
		public:
			HeightProfile(Interval interval, Height height, Scale scale = 1);
			HeightProfile(HeightProfile const& other);
			HeightProfile(HeightProfile const& other, Interval cutoutInterval);
			HeightProfile& operator=(HeightProfile& other);
			~HeightProfile();

			inline Interval GetInterval() const { return this->interval; }
			inline Height* GetHeightDataPtr() { return this->heightData; }

			inline Height& operator() (Coordinate x)
			{
				return this->heightData[x];
			}

			inline Height operator() (Coordinate x) const
			{
				return this->heightData[x];
			}

			inline Height operator() (double x)
			{
				Coordinate leftCoord = Coordinate(floor(x));
				Coordinate rightCoord = Coordinate(ceil(x));
				Height left = (*this)(leftCoord);
				Height right = (*this)(rightCoord);
				return Lerp(leftCoord, rightCoord, left, right, x);
			}

			inline Coordinate GetStart() const { return this->interval.GetStart(); }
			inline Size1D GetLength() const { return this->interval.GetLength(); }
			inline Scale GetScale() const { return this->scale; }

			inline Interval GetPhysicalIntervalUnscaled(Interval logicalInterval) const { return logicalInterval - this->interval.GetStart(); }
			inline Interval GetPhysicalInterval(Interval logicalInterval) const { return logicalInterval * this->scale - this->interval.GetStart(); }
			inline Coordinate GetPhysicalCoordinateUnscaled(Coordinate logicalPoint) const { return logicalPoint - this->interval.GetStart(); }
			inline Coordinate GetPhysicalCoordinate(Coordinate logicalPoint) const { return Coordinate(logicalPoint * this->scale - this->interval.GetStart()); }
			inline Size1D GetScaledSize(Size1D size) const { return Size1D(size * this->scale); }

			void Abs();
			void Add(Height addend);
			void AddMasked(Height addend, HeightProfile* mask);
			void AddProfile(HeightProfile* addend);
			void AddProfileMasked(HeightProfile* addend, HeightProfile* mask);
			void Blur(Size1D radius);
			void ClampHeights(Height min, Height max);
			void Combine(HeightProfile* other, HeightProfile* mask);
			void CropHeights(Height min, Height max, Height replace);
			//void Distort(HeightMap* distortionMap, Size1D maxDistance);
			void FillInterval(Interval fillInterval, Height height);
			void FromArray(std::map<Coordinate, Height> const& heights);
			void Gradient(Coordinate source, Coordinate destination, Height fromHeight, Height toHeight, bool fillOutside);
			void Intersect(HeightProfile* other);
			void Invert();
			void Move(Coordinate offset);
			void Multiply(Height factor);
			void MultiplyProfile(HeightProfile* factor);
			void Noise(std::vector<NoiseLayer> layers, random::RandomSeed seed);
			//void Repeat(Rectangle repeatRectangle);
			void Rescale(Scale scale);
			//void SelectHeights(Height min, Height max);
			//void TransformValues(HeightProfile* function, Height min, Height max);
			void Unify(HeightProfile* other);
		};
	}
}