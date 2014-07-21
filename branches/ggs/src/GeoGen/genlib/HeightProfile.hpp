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
			HeightProfile(HeightProfile const& other);
			HeightProfile& operator=(HeightProfile& other);
			~HeightProfile();

			inline Interval GetInterval() const { return this->interval; }
			inline Height* GetHeightDataPtr() { return this->heightData; }

			void Add(Height height);
		};
	}
}