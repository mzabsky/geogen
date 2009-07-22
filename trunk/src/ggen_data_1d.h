/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

#include "ggen_amplitudes.h"

class GGen_Data_1D{
	public:
		int16* data;
		uint16 length;

		/* Constructors/destructors */
		GGen_Data_1D(uint16 length);
		GGen_Data_1D(uint16 length, int16 value);
		GGen_Data_1D(GGen_Data_1D& victim);
		~GGen_Data_1D();

		/* Basic data I/O */
		void SetValue(uint16 x, int16 value);
		void SetValueInRange(uint16 from, uint16 to, int16 value);
		int16 GetValue(uint16 x);
		int16 GetValue(uint16 x, uint16 scale_to_x);

		/* Elementary artihmetic and logic operations */
		void Add(int16 value);
		void Add(GGen_Data_1D* addend);
		void AddTo(int16 offset, GGen_Data_1D* addend);
		void AddMasked(int value, GGen_Data_1D* mask, bool relative);
		void AddMasked(GGen_Data_1D* addend, GGen_Data_1D* mask, bool relative);
		void Multiply(double value);
		void Multiply(GGen_Data_1D* factor);
		void Invert();
		void Scale(double ratio, bool scale_values);
		void ScaleTo(uint16 new_length, bool scale_values);
		void ScaleValuesTo(int16 new_min, int16 new_max);
		void Fill(int16 value);
		void ResizeCanvas(int16 new_length, int16 new_zero);
		void Clamp(int16 min, int16 max);
		void Flip();
		int16 Min();
		int16 Max();
		void Shift(int16 distance, GGen_Overflow_Mode mode);
		void Union(GGen_Data_1D* unifiee);
		void Intersection(GGen_Data_1D* intersectee);
		
		/* Advanced operations with array data */
		void Monochrome(int16 treshold);
		void Normalize(GGen_Normalization_Mode mode);
		void SlopeMap();
		void Gradient(uint16 from, uint16 to, int16 from_value, int16 to_value, bool fill_flat);
		void Noise(uint16 min_feature_size, uint16 max_feature_size, GGen_Amplitudes* amplitudes);
		void Noise(uint16 min_feature_size, uint16 max_feature_size);
		void Smooth(uint8 radius);
		void Flood(double water_amount);

		/* Human interface functions */
		void Print();
		void Window(uint16 height);
};