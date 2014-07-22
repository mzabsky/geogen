 #pragma once

#include "../Number.hpp"
#include "../Rectangle.hpp"
#include "DataObject.hpp"

namespace geogen
{
	namespace genlib
	{
		class HeightMap : public DataObject
		{
		private:
			Rectangle rectangle;
			Height* heightData;
		public:
			HeightMap(Rectangle rectangle, Height height = 0);
			~HeightMap();
			HeightMap(HeightMap const& other);
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

			void Add(Height height);
			void AddMap(HeightMap* addend);
		};
	}	
}