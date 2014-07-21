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
			HeightMap(Rectangle rectangle);
			~HeightMap();
			HeightMap(HeightMap const& other);
			HeightMap& operator=(HeightMap& other);

			inline Rectangle GetRectangle() const { return this->rectangle; }
			inline Height* GetHeightDataPtr() { return this->heightData; }

			void Add(Height height);
		};
	}	
}