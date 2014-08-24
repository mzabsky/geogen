 #pragma once

#include <vector>

#include "../Number.hpp"
#include "../Rectangle.hpp"
#include "DataObject.hpp"
#include "../Direction.hpp"
#include "NoiseLayer.hpp"
#include "../random/RandomSeed.hpp"

namespace geogen
{
	namespace genlib
	{
		class HeightProfile;

		class HeightMap : public DataObject
		{
		private:
			Rectangle rectangle;
			Height* heightData;
			Scale scale;
		public:
			HeightMap(Rectangle rectangle, Height height = 0, Scale scale = 1);
			~HeightMap();
			HeightMap(HeightMap const& other);
			HeightMap(HeightMap const& other, Rectangle cutoutRect);
			HeightMap& operator=(HeightMap& other);

			inline Rectangle GetRectangle() const { return this->rectangle; }
			inline Height* GetHeightDataPtr() { return this->heightData; }

			inline Height& HeightMap::operator() (Coordinate x, Coordinate y)
			{
				return this->heightData[x + this->rectangle.GetSize().GetWidth() * y];
			}

			inline Height HeightMap::operator() (Coordinate x, Coordinate y) const
			{
				return this->heightData[x + this->rectangle.GetSize().GetWidth() * y];
			}

			inline Height& HeightMap::operator() (Point p)
			{
				return this->heightData[p.GetX() + this->rectangle.GetSize().GetWidth() * p.GetY()];
			}

			inline Height HeightMap::operator() (Point p) const
			{
				return this->heightData[p.GetX() + this->rectangle.GetSize().GetWidth() * p.GetY()];
			}

			inline Height HeightMap::operator() (double x, double y)
			{
				Coordinate leftCoord = Coordinate(floor(x));
				Coordinate rightCoord = Coordinate(ceil(x));
				Coordinate topCoord = Coordinate(floor(x));
				Coordinate bottomCoord = Coordinate(ceil(y));
				
				// TODO: Optimize.
				Height top = leftCoord == rightCoord ? (*this)(leftCoord, topCoord) : Lerp(leftCoord, rightCoord, (*this)(leftCoord, topCoord), (*this)(rightCoord, topCoord), x);
				Height bottom = leftCoord == rightCoord ? (*this)(leftCoord, bottomCoord) : Lerp(leftCoord, rightCoord, (*this)(leftCoord, bottomCoord), (*this)(rightCoord, bottomCoord), x);
				return topCoord == bottomCoord ? top : Lerp(topCoord, bottomCoord, top, bottom, y);
			}

			inline Coordinate GetOriginX() const { return this->rectangle.GetPosition().GetX(); }
			inline Coordinate GetOriginY() const { return this->rectangle.GetPosition().GetY(); }
			inline Size1D GetWidth() const { return this->rectangle.GetSize().GetWidth(); }
			inline Size1D GetHeight() const { return this->rectangle.GetSize().GetHeight(); }
			inline Scale GetScale() const { return this->scale; }

			inline Rectangle GetPhysicalRectangleUnscaled(Rectangle logicalRectangle) const { return logicalRectangle - this->rectangle.GetPosition(); }
			inline Rectangle GetPhysicalRectangle(Rectangle logicalRectangle) const { return logicalRectangle * this->scale - this->rectangle.GetPosition(); }
			inline Point GetPhysicalPointUnscaled(Point logicalPoint) const { return logicalPoint - this->rectangle.GetPosition(); }
			inline Point GetPhysicalPoint(Point logicalPoint) const { return logicalPoint * this->scale - this->rectangle.GetPosition(); }			
			inline Size1D GetScaledSize(Size1D size) const { return Size1D(size * this->scale); }
			
			void Abs();
			void Add(Height addend);
			void AddMasked(Height addend, HeightMap* mask);
			void AddMap(HeightMap* addend);
			void AddMapMasked(HeightMap* addend, HeightMap* mask);
			void Blur(Size1D radius);
			void Blur(Size1D radius, Direction direction);
			void ClampHeights(Height min, Height max);
			void Combine(HeightMap* other, HeightMap* mask);
			void ConvexityMap(Size1D radius);
			void CropHeights(Height min, Height max, Height replace);
			//void Distort(HeightMap* distortionMap, Size1D maxDistance);
			void FillRectangle(Rectangle fillRectangle, Height height);
			void Gradient(Point source, Point destination, Height fromHeight, Height toHeight);
			void Intersect(HeightMap* other);
			void Invert();
			void Move(Point offset);
			void Multiply(Height factor);
			void MultiplyMap(HeightMap* factor);
			void Noise(std::vector<NoiseLayer> layers, random::RandomSeed seed);
			//void NormalMap();
			//void Outline();
			void Projection(HeightProfile* profile, Direction direction);
			void RadialGradient(Point point, Size1D radius, Height fromHeight, Height toHeight);
			//void Repeat(Rectangle repeatRectangle);
			void Rescale(Scale horizontalScale, Scale verticalScale);
			void Shift(HeightProfile* profile, Size1D maximumDistance, Direction direction);
			//void SelectHeights(Height min, Height max);
			//void SlopeMap();
			//void Shift(HeightProfile* profile, Size1D maxDistance);
			//void TransformValues(HeightProfile* function, Height min, Height max);
			void Unify(HeightMap* other);

			// transforms
		};
	}	
}