 #pragma once

#include "../Number.hpp"
#include "DataObject.hpp"

namespace geogen
{
	namespace genlib
	{
		class HeightMap : public DataObject
		{
		private:
			int width, height;
			Height* heightData;
		public:
			HeightMap(int width, int height);
			void Add(Height height);
		};
	}	
}