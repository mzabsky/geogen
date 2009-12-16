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

#include "sqplus.h"
#include "ggen_support.h"

/**
 * GGen_Data_2D represents 2-dimensional array of values. Most straightforward interpretation of such array is a bitmap, where black represents some minimal value (usually 0) and white represents some maximal value (in case of 8 bit per channel bitmaps usually 255). You will probably realize many of the GGen_Data_2D functions strongly resemble functions from classical bitmap editors - <a href="#2d_add">adding</a> changes brightness, <a href="#2d_multiply">multiplying</a> changes contrast, <a href="#2d_transformvalues">curves</a>... Working with multiple 2D arrays with GGen is generally very similiar to working with layer enabled bitmap editor, just without a fancy GUI.
 **/
class GGen_Data_2D{
	public:
		GGen_Height* data;
		GGen_Size width;
		GGen_Size height;
		GGen_TotalSize length;

		/**
		 * Creates new GGen_Data_2D object of given size.
		 * @param width Width of the map.
		 * @param height Height of the map.
		 * @param value Default value for all cells.
		 */
		GGen_Data_2D(GGen_Size width, GGen_Size height, GGen_Height value);
		
		~GGen_Data_2D();

		/** 
		 * Creates a 1:1 copy of the current object.
		 * @return Copy of the object.
		 **/
		GGen_Data_2D* Clone();

		/** 
		 * Returns width of the map.
		 * @return Width of the map.
		 **/
		GGen_Size GetWidth();
		
		/** 
		 * Returns height of the map.
		 * @return height of the map.
		 **/
		GGen_Size GetHeight();

		/** 
		 * Returns length of the map.
		 * @return Length of the map.
		 * @return Length is total number of tiles in the map (= width * height)
		 **/
		GGen_Index GetLength();

		/** 
		 * Sets value in one tile.
		 * @param x X coordinate of the tile.
		 * @param y Y coordinate of the tile.
		 * @param value Value to use.
		 **/
		void SetValue(GGen_Coord x, GGen_Coord y, GGen_Height value);
		
		/** 
		 * Sets value in all tiles inside a rectangle (bounds are included).
		 * @param x1 X coordinate of the left border
		 * @param y1 Y coordinate of the top borer.
 		 * @param x2 X coordinate of the right border.
		 * @param y2 Y coordinate of the bottom border.
		 * @param value Value to use.
		 **/
		void SetValueInRect(GGen_Coord x1, GGen_Coord y1, GGen_Coord x2, GGen_Coord y2, GGen_Height value);
		
		/** 
		 * Returns one value from the array.
		 * @param x X coordinate of the tile.
		 * @param y Y coordinate of the tile.
		 * @return Value in the tile.
		 **/		
		GGen_Height GetValue(GGen_Coord x, GGen_Coord y);

		/** 
		 * Return one value from the array interpolated to a different map size.
		 * @param x X coordinate of the tile (in the interpolated map).
		 * @param y Y coordinate of the tile (in the interpolated map).
		 * @param scale_to_width Interpolated array width.
		 * @param scale_to_height Interpolated array height.
		 * @return Interpolated value in the tile.
		 * @note If the interpolated size is larger than the original, interpolated values will be calculated using the linear interpolation algorithm. Otherwise, interpolated values will be chosen using the nearest neighbor algorithm.
		 **/
		GGen_Height GetValueInterpolated(GGen_Coord x, GGen_Coord y, GGen_Size scale_to_width, GGen_Size scale_to_height);

		/** 
		 * Adds an integer to all values in the map.
		 * @param value Value to be added to values in the map.
		 **/
		void Add(GGen_Height value);

		/** 
		 * Adds another map to the current map.
		 * @param addend Map to be added. This map will be scaled to match the original map.
		 **/
		void AddMap(GGen_Data_2D* addend);

		/** 
		 * Adds another map to the current map. Its coordinates will be shifted by an offset.
		 * @param addend Map to be added. This map will NOT be scaled to match the original map.
		 * @param offset_x Coordinates of the addend will be shifted by this value along the X axis.
		 * @param offset_y Coordinates of the addend will be shifted by this value along the Y axis.
		 **/
		void AddTo(GGen_Data_2D* addend, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);
		
		/** 
		 * Adds a percentage of an integer to all values in the map. The percentage added will depend on corresponding value from the mask.
		 * @param value Value to be added to values in the map.
		 * @param mask Map used to mask the added value. This map will be scaled to match the current map.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask always means the original value won't be changed. In relative mode, maximum value found in the mask then means 100%, otherwise 255 means 100%.
		 **/			
		void AddMapMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative);
		
		/** 
		 * Adds another map to the current map. The percentage of each value from the addend will depend on corresponding value from the mask.
		 * @param addend Map to be added. This map will be scaled to match the original map.
		 * @param mask Map used to mask the added value.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask always means the original value won't be changed. In relative mode, maximum value found in the mask then means 100%, otherwise 255 means 100%.
		 **/			
		void AddMasked(GGen_Height value, GGen_Data_2D* mask, bool relative);
		
		/** 
		 * Multiplies each value in the map by a real number.
		 * @param multiplier Real number to multiply all values in the map.
		 **/		
		void Multiply(double factor);

		/** 
		 * Multiplies each value in the map by a corresponding value from factor.
		 * @param factor Array to be multiplied by. This map will be scaled to match the original map.
		 **/
		void MultiplyMap(GGen_Data_2D* factor);

		/** 
		 * Flips sign of all values in the map.
		 **/
		void Invert();

		/** 
		 * Scales size of the map by a real number.
		 * @param ratio Scaling ratio (0.5 = 50%, 2.0 = 200%).
		 * @param scale_values Multiply the values by the ratio as well?
		 * @note If the new size is larger than the original, the new values will be calculated using the linear interpolation algorithm. Otherwise, the new values will be chosen using the nearest neighbor algorithm.
		 **/
		void Scale(double ratio, bool scale_values);

		/** 
		 * Scales size of the map to new size.
		 * @param new_width Target array width.
		 * @param new_height Target array height.
		 * @param scale_values Scale the valuess correspondingly as well?
		 * @note If the new size is larger than the original, the new values will be calculated using the linear interpolation algorithm. Otherwise, the new values will be chosen using the nearest neighbor algorithm.
		 **/
		void ScaleTo(GGen_Size new_width, GGen_Size new_height, bool scale_values);
		
		/** 
		 * Scales values in the map to fit a new value range.
		 * @param new_min New minimum value.
		 * @param new_max New maximum value.
		 **/
		void ScaleValuesTo(GGen_Height new_min, GGen_Height new_max);
		
		/** 
		 * Crops or expands the array without changing its values.
		 * @param new_width New array width.
		 * @param new_height New array height.
		 * @param new_zero_x X coordinate of new origin relative to the original zero.
		 * @param new_zero_y Y coordinate of new origin relative to the original zero.
		 * @note All values outside the new map area will be discarded. Newly created values will be set to 0.
		 **/		
		void ResizeCanvas(GGen_Size new_width, GGen_Size new_height, GGen_CoordOffset new_zero_x, GGen_CoordOffset new_zero_y);
		
		/** 
		 * Sets all values in the map.
		 * @param value The fill value.
		 **/
		void Fill(GGen_Height value);

		/** 
		 * Clamps all values to range.
		 * @param min New minimum value.
		 * @param max New maximum value.
		 * @note All values outside the given range will be set either to min or max, whichever is closer.
		 **/
		void Clamp(GGen_Height min, GGen_Height max);

		/** 
		 * Returns the minimum of all values in the map.
		 * @return The minimum.
		 **/
		GGen_Height Min();

		/** 
		 * Returns the maximum of all values in the map.
		 * @return The maximum.
		 **/
		GGen_Height Max();

		/** 
		 * Performs a set intersection of the map graphs (higher of two respective values is applied).
		 * @param victim The intersection map. This map will be scaled to match the original map.
		 **/
		void Intersection(GGen_Data_2D* victim);
		void IntersectionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);

		/** 
		 * Performs a set union of the map graphs (higher of two respective values is applied).
		 * @param victim The union map. This map will be scaled to match the original map.
		 **/
		void Union(GGen_Data_2D* victim);
		void UnionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);
		void Combine(GGen_Data_2D* victim, GGen_Data_2D* mask, bool relative);
		void ReplaceValue(GGen_Height needle, GGen_Height replace);
		
		/** 
		 * Replaces each value with its absolute value (removes all negative signs).
		 **/
		void Abs();

		void Shift(GGen_Data_1D* profile, GGen_Direction direction, GGen_Overflow_Mode mode);
		void Project(GGen_Data_1D* profile, GGen_Direction direction);
		GGen_Data_1D* GetProfile(GGen_Direction direction, GGen_Coord coordinate);

		/**
		 * Creates a smooth gradient between two coords. The values will make transition between two values.
		 * @param from_x The starting point X coordinate.
		 * @param from_y The starting point Y coordinate.
		 * @param to_x The ending point X coordinate.
		 * @param to_y The starting point Y coordinate.
		 * @param from_value The starting transition base.
		 * @param to_value The ending transition base.
		 * @param fill_outside Should the values outside gradient area be filled as well?
		 * @note If fill_flat is set to true, values outside the gradient strip. Tiles closer to the starting point will be filled with from_value and tiles closer to the ending point will be filled with to_value.
		 **/
		void Gradient(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Height from_value, GGen_Height to_value, bool fill_outside);
		void GradientFromProfile(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Data_1D* pattern, bool fill_outside);
		void RadialGradient(GGen_Coord center_x, GGen_Coord center_y, GGen_Coord radius, GGen_Height min, GGen_Height max, bool fill_outside);
		void RadialGradientFromProfile(GGen_Coord center_x, GGen_Coord center_y, GGen_Distance radius, GGen_Data_1D* pattern, bool fill_outside);
		void Noise(GGen_Size min_feature_size, GGen_Size max_feature_size, GGen_Amplitudes* amplitudes);
		void VoronoiNoise(uint16 num_cells, uint8 points_per_cell, GGen_Voronoi_Noise_Mode mode);
		void SmoothDirection(GGen_Distance radius, GGen_Direction direction);
		void Smooth(GGen_Distance radius);
		
		/**
		 * Changes the values so goven percentage of values is higher than 0.
		 * @param land_amount The percentage of values to be higher than 0. 0 means no "land", 1.0 means no "water".
		 **/
		void Flood(double water_amount);
		void Pattern(GGen_Data_2D* pattern);

		/** 
		 * Replaces each value with 0 if it is less than equal than the treshold or 1 otherwise.
		 **/
		void Monochrome(GGen_Height treshold);
		void SelectValue(GGen_Height value);
		
		/**
		 * Replaces values in the array with information about steepness of slope (change in value) in that particular value.
		 **/
		void SlopeMap();
		void Scatter(bool relative);
		void TransformValues(GGen_Data_1D* profile, bool relative);
		
		/** 
		 * Makes sure that there are no slopes steeper than 45° in the map. Steeper slopes will be dealt with according to mode.
		 * @param mode The normalization mode (see GGen_Normalization_Mode).
		 **/
		void Normalize(GGen_Normalization_Mode mode);

		/** 
		 * Makes sure that there are no slopes steeper than 45° in either horizontal or vertical direction. Steeper slopes will be dealt with according to mode.
		 * @param direction Direction of the normalization.
		 * @param mode The normalization mode (see GGen_Normalization_Mode).
		 **/
		void NormalizeDirection(GGen_Direction direction, GGen_Normalization_Mode mode);
		
		/* Matrix operations */
		void Transform(double a11, double a12, double a21, double a22, bool preserve_size);
		void Rotate(int32 angle, bool preserve_size);
		void Shear(int32 horizontal_shear, int32 vertical_shear, bool preserve_size);
		void Flip(GGen_Direction direction);

		/* External communication functions */
		void ReturnAs(const SqPlus::sq_std_string &name);
};

