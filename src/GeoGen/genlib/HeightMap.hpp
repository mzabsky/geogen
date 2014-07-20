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
			void Add(Height height);
		};
	}	
}