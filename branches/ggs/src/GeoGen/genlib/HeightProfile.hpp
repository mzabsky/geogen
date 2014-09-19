#pragma once

#include <vector>
#include <map>

#include "../Interval.hpp"
#include "../Number.hpp"
#include "DataObject.hpp"
#include "NoiseLayersFactory.hpp"
#include "../random/RandomSeed.hpp"

namespace geogen
{
	namespace genlib
	{
		class HeightMap;

		/// 1-dimensional height data array. Profiles can be used to create and modify height maps using functions like HeightMap::Projection and HeightMap::Shift.
		class HeightProfile : public DataObject
		{
		private:
			Interval interval;
			Height* heightData;
			Scale scale;
		public:

			/// Creates a new profile with given parameters.
			/// @param interval The interval.
			/// @param height The height.
			/// @param scale (Optional) the scale.
			HeightProfile(Interval interval, Height height, Scale scale = 1);

			/// Copy constructor.
			/// @param other The other.
			HeightProfile(HeightProfile const& other);

			/// Copy constructor that only copies selected region of the profile.
			/// @param other The other.
			/// @param cutoutInterval The cutout interval.
			HeightProfile(HeightProfile const& other, Interval cutoutInterval);

			/// Assignment operator.
			/// @param other The other.
			/// @return A copy of this HeightProfile.
			HeightProfile& operator=(HeightProfile& other);

			/// Destructor.
			~HeightProfile();

			/// Gets the interval.
			/// @return The interval.
			inline Interval GetInterval() const { return this->interval; }

			/// Gets pointer to the internal height data array.
			/// @return Pointer to the height data.
			inline Height* GetHeightDataPtr() { return this->heightData; }

			/// Gets a height in pixel with given coordinate.
			/// @param x The coordinate.
			/// @return The result of the operation.
			inline Height& operator() (Coordinate x)
			{
				return this->heightData[x];
			}

			/// Gets a height in pixel with given coordinate.
			/// @param x The coordinate.
			/// @return The height.
			inline Height operator() (Coordinate x) const
			{
				return this->heightData[x];
			}

			/// Gets an interpolated height corresponding to given coordinate.
			/// @param x The coordinate.
			/// @return The height.
			inline Height operator() (double x)
			{
				Coordinate leftCoord = Coordinate(floor(x));
				Coordinate rightCoord = Coordinate(ceil(x));
				Height left = (*this)(leftCoord);
				Height right = (*this)(rightCoord);
				return Lerp(leftCoord, rightCoord, left, right, x);
			}

			/// Gets the starting coordinate.
			/// @return The starting coordinate.
			inline Coordinate GetStart() const { return this->interval.GetStart(); }

			/// Gets the length of the interval.
			/// @return The length.
			inline Size1D GetLength() const { return this->interval.GetLength(); }

			/// Gets the scale.
			/// @return The scale.
			inline Scale GetScale() const { return this->scale; }

			/// Gets an interval projected onto the physical data array without applying scaling on it.
			/// @param logicalInterval The logical interval to project.
			/// @return The projected interval.
			inline Interval GetPhysicalIntervalUnscaled(Interval logicalInterval) const { return logicalInterval - this->interval.GetStart(); }

			/// Gets an interval projected onto the physical data array with applying scaling on it.
			/// @param logicalInterval The logical interval to project.
			/// @return The projected interval.
			inline Interval GetPhysicalInterval(Interval logicalInterval) const { return logicalInterval * this->scale - this->interval.GetStart(); }
			
			/// Gets a coordinate projected onto the physical data array without applying scaling on it.
			/// @param logicalInterval The logical coordinate to project.
			/// @return The projected coordinate.
			inline Coordinate GetPhysicalCoordinateUnscaled(Coordinate logicalInterval) const { return logicalInterval - this->interval.GetStart(); }
			
			/// Gets a coordinate projected onto the physical data array with applying scaling on it.
			/// @param logicalInterval The logical coordinate to project.
			/// @return The projected coordinate.
			inline Coordinate GetPhysicalCoordinate(Coordinate logicalInterval) const { return Coordinate(logicalInterval * this->scale - this->interval.GetStart()); }

			/// Gets a coordinate projected from physical data array to logical coordinate space, reversing scaling.
			/// @param logicalInterval The physical coordinate to project.
			/// @return The projected coordinate.
			inline Coordinate GetLogicalCoordinate(Coordinate physicalCoordinate) const { return Coordinate((physicalCoordinate + this->interval.GetStart()) / this->scale); }

			/// Gets size scaled by the scaling factor of the profile.
			/// @param size The size to scale.
			/// @return The scaled size.
			inline Size1D GetScaledSize(Size1D size) const { return Size1D(size * this->scale); }

			/// Replaces value in each pixel with its absolute value.
			void Abs();

			/// Adds a number to each pixel in the height profile.
			/// @param addend The height to add.
			void Add(Height addend);

			/// Adds a number to each pixel in the height profile. The addition in each pixel will be modulated by the mask. Heights less than 0 from the mask are considered to be 0.
			/// @param addend The addend.
			/// @param The mask.
			void AddMasked(Height addend, HeightProfile* mask);

			/// Adds a corresponding pixel from another height profile to each pixel in the height profile.
			/// @param addend The addend.
			void AddProfile(HeightProfile* addend);

			/// Adds a corresponding pixel from another height profile to each pixel in the height profile. The addition in each pixel will be modulated by the mask. Heights less than 0 from the mask are considered to be 0.
			/// @param addend The addend.
			/// @param mask The mask.
			void AddProfileMasked(HeightProfile* addend, HeightProfile* mask);

			/// Blurs the height profile with kernel of given radius.
			/// @param radius The radius.
			void Blur(Size1D radius);

			/// Replaces heights greater than max with max and lesser than min with min.
			/// @param min The minimum.
			/// @param max The maximum.
			void ClampHeights(Height min, Height max);

			/// Replaces heights in each pixel with a height blended from that height and corresponding height from another height profile. 
			/// The blending will be done according to the height in the mask – 1 will mean 100% of the current height profile’s height will be used, 0 will mean 100% of the height from the other height profile will be used. Any heights in between will cause the two heights to be blended linearly. Heights less than 0 from the mask are considered to be 0.
			/// @param other The other profile.
			/// @param mask The mask.
			void Combine(HeightProfile* other, HeightProfile* mask);

			/// Replaces heights greater than max or lesser than min with replace (or 0 if not provided).
			/// @param min The minimum.
			/// @param max The maximum.
			/// @param replace The replacing height.
			void CropHeights(Height min, Height max, Height replace);

			//void Distort(HeightMap* distortionMap, Size1D maxDistance);

			/// Sets every pixel in interval to given height. 
			/// @param fillInterval The interval.
			/// @param height The height.
			void FillInterval(Interval fillInterval, Height height);

			/// Initializes this profile from array with key points.
			/// @param heights The key points.
			void FromArray(std::map<Coordinate, Height> const& heights);

			/// Gradients.
			/// @param source The source coordinate.
			/// @param destination The destination coordinate.
			/// @param fromHeight The height at source.
			/// @param toHeight The height at destination.
			/// @param fillOutside If set to true, the areas outside of the gradient interval will be set with the extreme heights, otherwise these areas will be left unchanged.
			void Gradient(Coordinate source, Coordinate destination, Height fromHeight, Height toHeight, bool fillOutside);

			/// Sets each pixel in the profile to the lesser of the two corresponding heights in the current profile and the other profile.
			/// @param other The other.
			void Intersect(HeightProfile* other);

			/// Flips the sign of each height in the height profile.
			void Invert();

			/// Moves the entire height profile by offset – a pixel at coordinate [x] will now be located at [x + offset].
			/// @param offset The offset.
			void Move(Coordinate offset);

			/// Multiplies the height in each pixel by a value corresponding to factor from <-1, 1> range. 
			/// @param factor The factor.
			void Multiply(Height factor);

			/// Multiplies the height in each pixel by a corresponding pixel from another height profile (mapped to range <-1, 1>). 
			/// @param [in,out] factor If non-null, the factor.
			void MultiplyProfile(HeightProfile* factor);

			/// Fills the map with random noise.
			/// @param layers The noise layers.
			/// @param seed The random seed.
			void Noise(NoiseLayers const& layers, random::RandomSeed seed);

			/// Creates a height profile filled with an interval of pattern repeated indefinitely in both directions. 
			/// @param pattern The pattern profile.
			/// @param repeatInterval The repeat interval.
			void Pattern(HeightProfile* pattern, Interval repeatInterval);

			/// Scales the entire height profile up (if the scale is greater than 1) or down (if the scale is less than 1) by given scale. Scale must be greater than 0.
			/// @param scale The scale.
			void Rescale(Scale scale);

			/// Creates a height profile from a 1 pixel wide slice of the height map located at coordinate in direction.
			/// @param heightMap The height map.
			/// @param direction The direction.
			/// @param coordinate The coordinate.
			void Slice(HeightMap* heightMap, Direction direction, Coordinate coordinate);

			//void SelectHeights(Height min, Height max);
			//void TransformValues(HeightProfile* function, Height min, Height max);

			/// Sets each pixel in the to the greater of the two corresponding heights in the current profile and the other profile.
			/// @param The other profile.
			void Unify(HeightProfile* other);
		};
	}
}