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

/** 
 * @file ggen_data_1d.h GGen_Data_1D represents a simple 1D array of values. It can be used to create more complex 2D maps using methods like GGen_Data_2D::Project, GGen_Data_2D::Gradient or GGen_Data_2D::TransformValues.
 **/

#pragma once

#include <cmath>
#include <cstring>

#include "ggen_amplitudes.h"
#include "ggen_point.h"
#include "ggen_path.h"

/**
 * GGen_Data_1D represents a simple 1D array of values. It can be used to create more complex 2D maps using methods like GGen_Data_2D::Project, GGen_Data_2D::Gradient or GGen_Data_2D::TransformValues.
 **/
class GGen_Data_1D{
	public:
		GGen_Height* data;
		GGen_Size length;

		/**
		 * Creates new GGen_Data_1D object of given length.
		 * @param length Length of the array.
		 * @param value Default value for all cells.
		 **/
		GGen_Data_1D(GGen_Size length, GGen_Height value);
		
		~GGen_Data_1D();

		/**
		 * Creates a 1:1 copy of the current object.
		 * @return Copy of the object.
		 **/
		GGen_Data_1D* Clone();

		/**
		 * Returns length of the array.
		 * @return Length of the array.
		 **/
		GGen_Size GetLength();

		/**
		 * Sets value in one tile.
		 * @param x Coordinate to set.
		 * @param value Value to use.
		 **/
		void SetValue(GGen_Coord x, GGen_Height value);

		/**
		 * Sets value in all tiles in range (bounds are included).
		 * @param from Starting point.
		 * @param to Ending point.
		 * @param value Value to use.
		 **/
		void SetValueInRange(GGen_Coord from, GGen_Coord to, GGen_Height value);

		/**
		 * Returns one value from the array.
		 * @param x Tile coordinate.
		 * @return Value in the tile.
		 **/
		GGen_Height GetValue(GGen_Coord x);
		
		/**
		 * Return one value from the array interpolated to a different array length.
		 * @param x Tile coordinate (in the interpolated array).
		 * @param scale_to_length Interpolated array length.
		 * @return Interpolated value in the tile.
		 * @note If the interpolated size is larger than the original, interpolated values will be calculated using the linear interpolation algorithm. Otherwise, interpolated values will be chosen using the nearest neighbor algorithm.
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
		 * @param offset Coordinates of the addend will be shifted by this value.
		 **/
		void AddTo(GGen_Data_1D* addend, GGen_CoordOffset offset);
		
		/**
		 * Adds a percentage of an integer to all values in the array. The percentage added will depend on corresponding value from the mask.
		 * @param value Value to be added to values in the array.
		 * @param mask Array used to mask the added value. This array will be scaled to match the current array.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask always means the original value won't be changed. In relative mode, maximum value found in the mask then means 100%, otherwise 32767 means 100%.
		 **/		
		void AddMasked(GGen_Height value, GGen_Data_1D* mask, bool relative);

		/**
		 * Adds another array to the current array. The percentage of each value from the addend will depend on corresponding value from the mask.
		 * @param addend Array to be added. This array will be scaled to match the original array.
		 * @param mask Array used to mask the added value.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask always means the original value won't be changed. In relative mode, maximum value found in the mask then means 100%, otherwise 32767 means 100%.
		 **/	
		void AddArrayMasked(GGen_Data_1D* addend, GGen_Data_1D* mask, bool relative);

		/**
		 * Multiplies each value in the array by a real number.
		 * @param factor Real number to multiply all values in the array.
		 **/
		void Multiply(double factor);

		/**
		 * Multiplies each value in the array by a corresponding value from factor.
		 * @param factor Array to be multiplied by. This array will be scaled to match the original array.
		 **/
		void MultiplyArray(GGen_Data_1D* factor);

		/**
		 * Flips sign of all values in the array.
		 **/
		void Invert();

		/**
		 * Scales length of the array by a real number.
		 * @param ratio Scaling ratio (0.5 = 50%, 2.0 = 200%).
		 * @param scale_values Multiply the values by the ratio as well?
		 * @note If the new size is larger than the original, the new values will be calculated using the linear interpolation algorithm. Otherwise, the new values will be chosen using the nearest neighbor algorithm.
		 **/
		void Scale(double ratio, bool scale_values);

		/**
		 * Scales length of the array to new length.
		 * @param new_length Target array length.
		 * @param scale_values Scale the valuess correspondingly as well?
		 * @note If the new size is larger than the original, the new values will be calculated using the linear interpolation algorithm. Otherwise, the new values will be chosen using the nearest neighbor algorithm.
		 **/
		void ScaleTo(GGen_Size new_length, bool scale_values);

		/**
		 * Scales values in the array to fit a new value range.
		 * @param new_min New minimum value.
		 * @param new_max New maximum value.
		 **/
		void ScaleValuesTo(GGen_Height new_min, GGen_Height new_max);

		/**
		 * Sets all values in the array.
		 * @param value The fill value.
		 **/
		void Fill(GGen_Height value);

		/**
		 * Crops or expands the array without changing its values.
		 * @param new_length New array length.
		 * @param new_zero Coordinate of new origin relative to the original zero.
		 * @note All values outside new length will be discarded. Newly created values will be set to 0.
		 **/
		void ResizeCanvas(GGen_Size new_length, GGen_CoordOffset new_zero);

		/**
		 * Clamps all values to range.
		 * @param min New minimum value.
		 * @param max New maximum value.
		 * @note All values outside the given range will be set either to min or max, whichever is closer.
		 **/
		void Clamp(GGen_Height min, GGen_Height max);

		/**
		 * Flips order of values in the array, so the first value is the last.
		 **/
		void Flip();

		/**
		 * Returns the minimum of all values in the array.
		 * @return The minimum.
		 **/
		GGen_Height Min();

		/**
		 * Returns the maximum of all values in the array.
		 * @return The maximum.
		 **/
		GGen_Height Max();

		/**
		 * Shifts all values by a distance.
		 * @param offset Coordinate of new origin relative to the original zero.
		 * @param overflow_mode Overflow mode (see GGen_Overflow_Mode).
 		 * @note All values outside the array will be discarded. Newly created values will be set to 0.
		 **/
		void Shift(GGen_CoordOffset offset, GGen_Overflow_Mode overflow_mode);

		/**
		 * Performs a set union of the array graphs (higher of two respective values is applied).
		 * @param victim The union array. This array will be scaled to match the original array.
		 **/
		void Union(GGen_Data_1D* victim);
		
		/**
		 * Performs a set intersection of the array graphs (higher of two respective values is applied).
		 * @param victim The intersection array. This array will be scaled to match the original array.
		 **/
		void Intersection(GGen_Data_1D* victim);

		/**
		 * Replaces each value with its absolute value (removes all negative signs).
		 **/
		void Abs();
		
		/**
		 * Replaces each value with 0 if it is less than equal than the threshold or 1 otherwise.
		 * @param threshold Values above threshold will be 1, otherwise 0.
		 **/
		void Monochrome(GGen_Height threshold);
		
		/**
		 * Makes sure that there are no slopes steeper than 45° in the array. Steeper slopes will be dealt with according to mode.
		 * @param mode The normalization mode (see GGen_Normalization_Mode).
		 **/
		void Normalize(GGen_Normalization_Mode mode);

		/**
		 * Replaces values in the array with information about steepness of slope (change in value) in that particular value.
		 **/
		void SlopeMap();
		
		/**
		 * Creates a smooth gradient between two coords. The values will make transition between two values.
		 * @param from The left bound coordinate.
		 * @param to The right bound coordinate.
		 * @param from_value The left transition base.
		 * @param to_value The right transition base.
		 * @param fill_outside Should the values outside gradient interval be filled as well?
		 * @note If fill_flat is set to true, values with coord lower than left bound will be filled with from_value< and value with coord higher than the right bound will be filled with to_value.
		 **/
		void Gradient(GGen_Coord from, GGen_Coord to, GGen_Height from_value, GGen_Height to_value, bool fill_outside);
		
		/**
		 * Fills the array with random perlin noise (http://freespace.virgin.net/hugo.elias/models/m_perlin.htm).
		 * @param min_feature_size Minimum wave length for amplitude to be used.
		 * @param max_feature_size Maximum wave length for amplitude to be used.
		 * @param amplitudes GGen_Amplitudes object.
		 * @note Resulting computational complexity increases with number of amplitudes used.
		 **/
		void Noise(GGen_Size min_feature_size, GGen_Size max_feature_size, GGen_Amplitudes* amplitudes);
		
		/**
		 * Smooths differences between values in the array. Uses linear smoothing algorithm.
		 * @param radius Smoothing kernel radius.
		 **/
		void Smooth(GGen_Distance radius);

		/**
		 * Changes the values so given percentage of values is higher than 0.
		 * @param land_amount The percentage of values to be higher than 0. 0 means no "land", 1.0 means no "water".
		 **/
		void Flood(double land_amount);

		/**
		 * Creates a GGen_Path object from current object.
		 * @param point_count Number of points in the newly created path object.
		 * @note Point's coordinates are calculated as [value index, value].
		 **/
		GGen_Path* ToPath(uint16 point_count);
};