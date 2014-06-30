#pragma once

#include "../Interval.hpp"
#include "../Number.hpp"
#include "DataObject.hpp"

namespace geogen
{
	namespace genlib
	{
		class HeightProfile : public DataObject
		{
		private:
			Interval interval;
			Height* heightData;
		public:
			HeightProfile(Interval interval);
			~HeightProfile();
			void Add(Height height);
		};
	}
}