#pragma once

#include <string>
#include <ostream>

#include "../Number.hpp"

namespace geogen
{
	namespace genlib
	{
		class HeightMap
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