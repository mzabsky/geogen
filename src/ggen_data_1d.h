/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
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
		GGen_Height* data;
		GGen_Size length;

		/**
		 * Creates new GGen_Data_1D object of given length.
		 * @param length of the array
		 * @param default value of all cells
		 */
		GGen_Data_1D(GGen_Size length, GGen_Height value);
		
		~GGen_Data_1D();

		/** 
		 * Creates a 1:1 copy of original.
		 * @return copy of thee object
		 **/
		GGen_Data_1D* Clone();

		/** 
		 * Returns length of the array.
		 * @param length of the array
		 **/
		GGen_Size GetLength();
		void SetValue(GGen_Coord x, GGen_Height value);
		void SetValueInRange(GGen_Coord from, GGen_Coord to, GGen_Height value);
		GGen_Height GetValue(GGen_Coord x);
		GGen_Height GetValueInterpolated(GGen_Coord x, GGen_Size scale_to_length);

		/* Elementary artihmetic and logic operations */
		void Add(GGen_Height value);
		void AddArray(GGen_Data_1D* addend);
		void AddTo(GGen_Data_1D* addend, GGen_CoordOffset offset);
		void AddMasked(GGen_Height value, GGen_Data_1D* mask, bool relative);
		void AddArrayMasked(GGen_Data_1D* addend, GGen_Data_1D* mask, bool relative);
		void Multiply(double value);
		void MultiplyArray(GGen_Data_1D* factor);
		void Invert();
		void Scale(double ratio, bool scale_values);
		void ScaleTo(GGen_Size new_length, bool scale_values);
		void ScaleValuesTo(GGen_Height new_min, GGen_Height new_max);
		void Fill(GGen_Height value);
		void ResizeCanvas(GGen_Size new_length, GGen_CoordOffset new_zero);
		void Clamp(GGen_Height min, GGen_Height max);
		void Flip();
		GGen_Height Min();
		GGen_Height Max();
		void Shift(GGen_CoordOffset distance, GGen_Overflow_Mode mode);
		void Union(GGen_Data_1D* victim);
		void Intersection(GGen_Data_1D* victim);
		void Abs();
		
		/* Advanced operations with array data */
		void Monochrome(GGen_Height treshold);
		void Normalize(GGen_Normalization_Mode mode);
		void SlopeMap();
		void Gradient(GGen_Coord from, GGen_Coord to, GGen_Height from_value, GGen_Height to_value, bool fill_flat);
		void Noise(GGen_Size min_feature_size, GGen_Size max_feature_size, GGen_Amplitudes* amplitudes);
		void Smooth(GGen_Distance radius);
		void Flood(double water_amount);
};