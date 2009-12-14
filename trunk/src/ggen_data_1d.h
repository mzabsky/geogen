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
		 * @param legth Length of the array.
		 * @param value Default value for all cells.
		 */
		GGen_Data_1D(GGen_Size length, GGen_Height value);
		
		~GGen_Data_1D();

		/** 
		 * Creates a 1:1 copy of the current object.
		 * @return Copy of the object.
		 **/
		GGen_Data_1D* Clone();

		/** 
		 * Returns length of the array.
		 * @return Length of the array
		 **/
		GGen_Size GetLength();

		/** 
		 * Sets value in one tile.
		 * @param coordinate to set
		 * @param value to use
		 **/
		void SetValue(GGen_Coord x, GGen_Height value);

		/** 
		 * Sets value in all tiles in range (bounds are included).
		 * @param starting point
		 * @param ending point
		 * @param value to use
		 **/
		void SetValueInRange(GGen_Coord from, GGen_Coord to, GGen_Height value);

		/** 
		 * Returns one value from the array.
		 * @param tile coordinate
		 * @return value in the tile
		 **/
		GGen_Height GetValue(GGen_Coord x);
		
		/** 
		 * Return one value from the array interpolated to a different array length.
		 * @param x Tile coordinate (in the interpolated array).
		 * @param scale_to_length Interpolated array length.
		 * @return Interpolated value in the tile.
		 **/
		GGen_Height GetValueInterpolated(GGen_Coord x, GGen_Size scale_to_length);

		/** 
		 * Adds an integer to all values in the array.
		 * @param value Value to be added to values in the array.
		 **/
		void Add(GGen_Height value);

		/** 
		 * Adds another array to the current array.
		 * @param addend Array to be added. This array will be scaled to match the original array.
		 **/
		void AddArray(GGen_Data_1D* addend);

		/** 
		 * Adds another array to the current array. Its coordinates will be shifted by an offset.
		 * @param addend Array to be added. This array will NOT be scaled to match the original array.
		 * @param offfset Coordinates of the addend will be shifted by this value.
		 **/
		void AddTo(GGen_Data_1D* addend, GGen_CoordOffset offset);
		
		/** 
		 * Adds a percentage of an integer to all values in the array. The percentage added will depend on corresponding value from the mask.
		 * @param value Value to be added to values in the array.
		 * @param mask Array used to mask the added value. This array will be scaled to match the current array.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask always means the original value won't be changed. In relative mode, maximum value found in the mask then means 100%, otherwise 255 means 100%.
		 **/		
		void AddMasked(GGen_Height value, GGen_Data_1D* mask, bool relative);

		/** 
		 * Adds another array to the current array. The percentage of each value from the addend will depend on corresponding value from the mask.
		 * @param value Value to be added to values in the array.
		 * @param mask Array used to mask the added value.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask always means the original value won't be changed. In relative mode, maximum value found in the mask then means 100%, otherwise 255 means 100%.
		 **/	
		void AddArrayMasked(GGen_Data_1D* addend, GGen_Data_1D* mask, bool relative);

		/** 
		 * Multiplies each value in the array by a real number.
		 * @param multiplier Real number to multiply all values in the array.
		 **/
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