 #pragma once

#include <vector>

#include "../Number.hpp"
#include "../Rectangle.hpp"
#include "DataObject.hpp"
#include "../Orientation.hpp"
#include "NoiseLayer.hpp"
#include "../random/RandomSeed.hpp"

namespace geogen
{
	namespace genlib
	{
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

			inline Coordinate GetOriginX() const { return this->rectangle.GetPosition().GetX(); }
			inline Coordinate GetOriginY() const { return this->rectangle.GetPosition().GetY(); }
			inline Size1D GetWidth() const { return this->rectangle.GetSize().GetWidth(); }
			inline Size1D GetHeight() const { return this->rectangle.GetSize().GetHeight(); }
			inline Scale GetScale() const { return this->GetScale(); }

			inline Rectangle GetPhysicalRectangleUnscaled(Rectangle logicalRectangle) const { return logicalRectangle - this->rectangle.GetPosition(); }
			inline Rectangle GetPhysicalRectangle(Rectangle logicalRectangle) const { return logicalRectangle * this->scale - this->rectangle.GetPosition(); }
			inline Point GetPhysicalPointUnscaled(Point logicalPoint) const { return logicalPoint - this->rectangle.GetPosition(); }
			inline Point GetPhysicalPoint(Point logicalPoint) const { return logicalPoint * this->scale - this->rectangle.GetPosition(); }			

			void Add(Height height);
			void AddMap(HeightMap* addend);
			void RadialGradient(Point point, Size1D radius, Height fromHeight, Height toHeight);
			void Blur(Size1D radius);
			void Blur(Size1D radius, Orientation direction);
			void FillRectangle(Rectangle fillRectangle, Height height);
			void Noise(std::vector<NoiseLayer> layers, random::RandomSeed seed);
		};
	}	
}