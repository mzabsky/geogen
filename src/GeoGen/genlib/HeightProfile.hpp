#pragma once

#include "../Point.hpp"
#include "../Size.hpp"
#include "../Number.hpp"
#include "DataObject.hpp"

namespace geogen
{
	namespace genlib
	{
		class HeightProfile : public DataObject
		{
		private:
			Coordinate origin;
			Size1D length;
			Height* heightData;
		public:
			HeightProfile(Size1D length) : HeightProfile(0, length) {};
			HeightProfile(Coordinate origin, Coordinate length);
			~HeightProfile();
			void Add(Height height);
		};
	}
}