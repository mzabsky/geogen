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

#include "ggen_support.h"
#include "ggen_path.h"
#include "ggen_data_1d.h"
#include <iostream>
#include <set>

using namespace std;

/**
 * GGen_Data_2D represents 2-dimensional array of values. Most straightforward interpretation of such array is a bitmap, where black represents some minimal value (usually 0) and white represents some maximal value (in case of 8 bit per channel bitmaps usually 255). You will probably realize many of the GGen_Data_2D functions strongly resemble functions from classical bitmap editors - <a href="#2d_add">adding</a> changes brightness, <a href="#2d_multiply">multiplying</a> changes contrast, <a href="#2d_transformvalues">curves</a>... Working with multiple 2D arrays with GGen is generally very similiar to working with layer enabled bitmap editor, just without a fancy GUI.
 **/
class GGen_Data_2D{
	protected:
		static uint16 num_instances;
		static set<GGen_Data_2D*> instances;

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
		 **/
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
		 * @note Length is total number of tiles in the map (= width * height)
		 **/
		GGen_Index GetLength();

		/**
		 * Returns maximum possible distance between two cells within the map.
		 * @return Maximum distance within the map.
		 **/
		GGen_Distance GetMaxDistance();

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
		 * Adds another map to the current map. The percentage of each value from the addend will depend on corresponding value from the mask.
		 * @param addend Map to be added. This map will be scaled to match the original map.
		 * @param mask Map used to mask the added value.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask always means the original value won't be changed. In relative mode, maximum value found in the mask then means 100%, otherwise 32767 means 100%.
		 **/
		void AddMapMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative);

		/**
		 * Adds a percentage of an integer to all values in the map. The percentage added will depend on corresponding value from the mask.
		 * @param value Value to be added to values in the map.
		 * @param mask Map used to mask the added value. This map will be scaled to match the current map.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask always means the original value won't be changed. In relative mode, maximum value found in the mask then means 100%, otherwise 32767 means 100%.
		 **/
		void AddMasked(GGen_Height value, GGen_Data_2D* mask, bool relative);
		
		/**
		 * Multiplies each value in the map by a real number.
		 * @param factor Real number to multiply all values in the map.
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
		 * @param scale_values Scale the values correspondingly as well?
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
		 * Sets all values in the map. The percentage of value changed in each tile will depend on corresponding values in the mask.
		 * @param value The fill value.
		 * @param mask Map used to mask the filled value. This map will be scaled to match the current map.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask always means the original value won't be changed. In relative mode, maximum value found in the mask then means 100%, otherwise 32767 means 100%.
		 **/
		void FillMasked(GGen_Height value, GGen_Data_2D* mask, bool relative);

		/**
		 * Clamps all values to range. All values outside the given range will be set either to min or max, whichever is closer.
		 * @param min New minimum value.
		 * @param max New maximum value.
		 **/
		void Clamp(GGen_Height min, GGen_Height max);

		/**
		 * Clamps all values to range. All values outside the given range will be set to 0.
		 * @param min New minimum value.
		 * @param max New maximum value.
		 **/
		void CropValues(GGen_Height min, GGen_Height max);

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

		/**
		 * Performs a set intersection of the map graphs (higher of two respective values is applied). The intersection map coordinates will be shifted by an offset.
		 * @param victim The intersection map. This map will NOT be scaled to match the original map.
		 * @param offset_x Coordinates of the intersection map will be shifted by this value along the X axis.
		 * @param offset_y Coordinates of the intersection map will be shifted by this value along the Y axis.
		 **/
		void IntersectionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);

		/**
		 * Performs a set union of the map graphs (higher of two respective values is applied).
		 * @param victim The union map. This map will be scaled to match the original map.
		 **/
		void Union(GGen_Data_2D* victim);

		/**
		 * Performs a set union of the map graphs (higher of two respective values is applied). The union map coordinates will be shifted by an offset.
		 * @param victim The union map. This map will NOT be scaled to match the original map.
		 * @param offset_x Coordinates of the intersection map will be shifted by this value along the X axis.
		 * @param offset_y Coordinates of the intersection map will be shifted by this value along the Y axis.
		 **/
		void UnionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);

		/**
		 * Combines the current map with another map according to corresponding values in mask.
		 * @param victim The intersection map. This map will NOT be scaled to match the original map.
		 * @param victim The second map. This map will be scaled to match the original map.
		 * @param mask The mask used to determine value percentage from each map used. This map will NOT be scaled to match the original map.
		 * @param relative Toggles relative mode.
		 * @note 0 in the mask means the second map value will be used. In relative mode, maximum value (32767 otherwise) found in the mask then means the current map value will be used. Other mask values will mean a combination of both map values (the higher the mask value, the more of the current array value will be used).
		 **/
		void Combine(GGen_Data_2D* victim, GGen_Data_2D* mask, bool relative);

		/**
		 * Replaces all occurrences of a value with a different value.
		 * @param needle The replaced value.
		 * @param replace The new value.
		 **/
		void ReplaceValue(GGen_Height needle, GGen_Height replace);
		
		/**
		 * Replaces each value with its absolute value (removes all negative signs).
		 **/
		void Abs();

		/**
		 * Shifts all values in the array towards a direction by a corresponding value from profile.
		 * @param profile The shift profile.
		 * @param direction Direction towards which will the shiting be done.
		 * @param mode The overflow mode (see GGen_Overflow_Mode).
		 **/
		void Shift(GGen_Data_1D* profile, GGen_Direction direction, GGen_Overflow_Mode mode);

		/**
		 * Extrudes one-dimensional profile towards a direction.
		 * @param profile The profile to be projected.
		 * @param direction The extrusion direction.
		 **/
		void Project(GGen_Data_1D* profile, GGen_Direction direction);

		/**
		 * Extracts a one-dimensional slice from the map. 
		 * @param direction Determines if a row (GGEN_HORIZONTAL) or column will be extracted.
		 * @param coordinate Coordinate of the row/column.
		 **/
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
		 * @note If fill_flat is set to true, values outside the gradient strip will be filled with flat color. Tiles closer to the starting point will be filled with from_value and tiles closer to the ending point will be filled with to_value.
		 **/
		void Gradient(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Height from_value, GGen_Height to_value, bool fill_outside);

		/**
		 * Creates a smooth gradient between two coords. The values will be picked from the gradient profile according to the point's position on the gradient.
		 * @param from_x The starting point X coordinate.
		 * @param from_y The starting point Y coordinate.
		 * @param to_x The ending point X coordinate.
		 * @param to_y The starting point Y coordinate.
		 * @param profile The gradient profile.
		 * @param fill_outside Should the values outside gradient area be filled as well?
		 * @note If fill_flat is set to true, values outside the gradient strip will be filled with flat color. Tiles closer to the starting point will be filled with the left-most value from the profile and tiles closer to the ending point will be filled with the right-most value from the profile.
		 **/
		void GradientFromProfile(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Data_1D* profile, bool fill_outside);
		
		/**
		 * Creates a radial gradient. The values will make a smooth transition between the center and the outer rim.
		 * @param center_x X coordinate of the gradient center.
		 * @param center_y Y coordinate of the gradient center.
		 * @param radius The gradient radius.
		 * @param from_value Value in the center.
		 * @param to_value Value on the outer rim.
		 * @param fill_outside Should the values outside gradient area be filled as well?
		 * @note If fill_flat is set to true, values beyond the outer rim will be filled with to_value.
		 **/
		void RadialGradient(GGen_Coord center_x, GGen_Coord center_y, GGen_Coord radius, GGen_Height from_value, GGen_Height to_value, bool fill_outside);
		
		/**
		 * Creates a radial gradient. The values will be picked from the gradient profile according to the point's position on the gradient.
		 * @param center_x X coordinate of the gradient center.
		 * @param center_y Y coordinate of the gradient center.
		 * @param radius The gradient radius.
		 * @param profile The gradient profile.
		 * @param fill_outside Should the values outside gradient area be filled as well?
		 * @note If fill_flat is set to true, values beyond the outer rim will be filled with the right-most value from the profile.
		 **/
		void RadialGradientFromProfile(GGen_Coord center_x, GGen_Coord center_y, GGen_Distance radius, GGen_Data_1D* profile, bool fill_outside);
		
		/**
		 * Fills the array with random fractal noise.
		 * @param min_feature_size Minimum wave length for amplitude to be used.
		 * @param max_feature_size Maximum wave length for amplitude to be used.
		 * @param amplitudes GGen_Amplitudes object.
		 **/
		void Noise(GGen_Size min_feature_size, GGen_Size max_feature_size, GGen_Amplitudes* amplitudes);

		/**
		 * Fills the array with random voronoi noise.
		 * @param cell_size Size of one cell (cells are considered to be squares).
		 * @param points_per_cell Number of points randomly placed in each cell.
		 * @param mode The noise mode (see GGen_Voronoi_Noise_Mode).
		 * @note Computational complexity steeply rises with points_per_cell.
		 * @note Ratio of the points_per_cell and cell_size determines randomness and density of the noise.
		 **/
		void VoronoiNoise(GGen_Size cell_size, uint8 points_per_cell, GGen_Voronoi_Noise_Mode mode);

		/**
		 * Blurs the map in one direction. Uses linear smoothing algorithm.
		 * @param radius The smoothing kernel radius.
		 * @param direction Direction in which is the smoothing done.
		 **/
		void SmoothDirection(GGen_Distance radius, GGen_Direction direction);
		
		/**
		 * Blurs the map. Uses linear smoothing algorithm.
		 * @param radius The smoothing kernel radius.
		 **/
		void Smooth(GGen_Distance radius);
		
		/**
		 * Changes the values so only given percentage of values is higher than 0.
		 * @param land_amount The percentage of values to be higher than 0. 0 means no "land", 1.0 means no "water".
		 **/
		void Flood(double land_amount);

		/**
		 * Fills the current map with repeating pattern.
		 * @param pattern The pattern.
		 **/
		void Pattern(GGen_Data_2D* pattern);

		/**
		 * Replaces each value with 0 if it is less than equal than the threshold or 1 otherwise.
		 * @param threshold Values above threshold will be 1, otherwise 0.
		 **/
		void Monochrome(GGen_Height threshold);

		/**
		 * Replaces all occurrences of a value with 1, all other values will be replaced with 0. 
		 * @param value The selected value.
		 **/
		void SelectValue(GGen_Height value);
		
		/**
		 * Replaces values in the array with information about steepness of slope (change in value) in that particular value.
		 **/
		void SlopeMap();

		/**
		 * Replaces each value with 1 with a probability defined by its value, all other values will be replaced with 1.
		 * @param relative Toggles relative mode.
		 * @note Value 0 means 0% probability to create a 1. In relative mode, maximum value (32767 otherwise) found in the map then means 100% probability to create a 1.
		 **/
		void Scatter(bool relative);

		/**
		 * Replaces each value with a corresponding value from profile. Coordinate of the corresponding value is determined by the original value - the higher the value, the higher the coordinate in profile is. 
		 * @param profile The transformation profile.
		 * @param relative Toggles relative mode.
		 * @note Only values higher than zero are affected. 
		 * @note If relative is false, then maximum value corresponds to the rightmost coordinate (0 always corresponds to 0), else the values are used in range 0-32767 only.
		 * @note The input profile will be internally smoothed to prevent sharp steps on the transformed terrain.
		 **/
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
		
		/**
		 * Applies a linear transformation matrix onto the map.
		 * @param a11 Matrix element (1,1).
		 * @param a12 Matrix element (1,2).
		 * @param a21 Matrix element (2,1).
		 * @param a22 Matrix element (2,2).
		 * @pre The matrix must be invertible (a11 * a22 - a12 * a21 != 0).
		 * @param preserve_size If set to true, the result will be cropped/expanded to match its original boundaries.
		 * @note Do not use this function to scale the map, it uses a low-quality nearest neighbor interpolation.
		 **/
		void Transform(double a11, double a12, double a21, double a22, bool preserve_size);

		/**
		 * Rotates the map by an angle counter-clockwise.
		 * @param angle The angle in degrees.
		 * @param preserve_size If set to true, the result will be cropped to match its original boundaries.
		 **/
		void Rotate(int32 angle, bool preserve_size);
		
		/**
		 * Shears the map vertically and/or horizontally.
		 * @param horizontal_shear Horizontal shear factor (0 means no horizontal shearing).
		 * @param vertical_shear Horizontal shear factor (1 means no horizontal shearing).
		 * @param preserve_size If set to true, the result will be cropped to match its original boundaries.
		 * @pre horizontal_shear != 1 or vertical_shear != 1.
		 **/
		void Shear(int32 horizontal_shear, int32 vertical_shear, bool preserve_size);

		/**
		 * Flips the map along one coordinate axis.
		 * @param direction The axis, along which the map will be flipped.
		 **/
		void Flip(GGen_Direction direction);

		/**
		 * Calls the API return handler.
		 * @param label Label identifying the returned map.
		 **/
		void ReturnAs(const GGen_String& label);

		/**
		 * Fills a polygon defined by its outer path.
		 * @param path Sequence of points defining the polygon's shape. The polygon is enclosed by connecting the first and last points of the sequence.
		 * @param value Value to be filled with.
		 * @note Filled areas are determined by odd-even rule.
		 **/
		void FillPolygon(GGen_Path* path, GGen_Height value);

		/**
		 * Paints a path with one-dimensional brush onto the map.
		 * @param path The path to be stroked.
		 * @param brush Brush used. Value with index 0 will be the closest to the path itself.
		 * @param radius Resulting size of the stroke.
		 * @param fill_outside Fill the area outside the brush stroke?
		 * @note Computational complexity of this function directly depends on number of segments in the path.
		 **/
		void StrokePath(GGen_Path* path, GGen_Data_1D* brush, GGen_Distance radius, bool fill_outside);

		void FloodFillBase(GGen_Coord start_x, GGen_Coord start_y, GGen_Height fill_value, GGen_Comparison_Mode mode, GGen_Height threshold, bool select_only);	

		/**
		 * Fills uniform area matching a simple arithmetic condition (all tiles matching the condition reachable from the starting point through tiles matching the condition are filled with value).
		 * @param start_x X coordinate of the starting point.
		 * @param start_y Y coordinate of the starting point.
		 * @param fill_value The value the matching area is filled with.
		 * @param mode Arithmetic operator to be used in the condition.
		 * @param threshold Value to be compared against.
		 * @note If the starting point doesn't match the condition, no tiles are changed.
		 * @note The filling is 4-directional (the spread is only along the main two axes).
		 **/
		void FloodFill(GGen_Coord start_x, GGen_Coord start_y, GGen_Height fill_value, GGen_Comparison_Mode mode, GGen_Height threshold);

		/**
		 * Replaces all values in an uniform area matching a simple arithmetic condition with 1 (all tiles matching the condition reachable from the starting point through tiles matching the condition are filled with 1). All other areas are filled with 0.
		 * @param start_x X coordinate of the starting point.
		 * @param start_y Y coordinate of the starting point.
		 * @param mode Arithmetic operator to be used in the condition.
		 * @param threshold Value to be compared against.
		 * @note If the starting point doesn't match the condition, all tiles will be filed with 0.
		 * @note The filling is 4-directional (the spread is only along the main two axes).
		 **/
		void FloodSelect(GGen_Coord start_x, GGen_Coord start_y, GGen_Comparison_Mode mode, GGen_Height threshold);

		GGen_Height GetValueOnPathBase(GGen_Path* path, bool max);

		/**
		 * Returns the highest value found in tiles touching a path.
		 * @param path Sequence of points defining the path.
		 * @return The maximum value found on the path.
		 **/
		GGen_Height GetMaxValueOnPath(GGen_Path* path);

		/**
		 * Returns the lowest value found in tiles touching a path.
		 * @param path Sequence of points defining the path.
		 * @return The minimum value found on the path.
		 **/
		GGen_Height GetMinValueOnPath(GGen_Path* path);

		void ExpandShrinkDirectionBase(GGen_Distance distance, GGen_Direction direction, bool shrink);

		/**
		 * Fills all areas within a distance in one direction from any value greater than 0 with 1. The rest of the map will be filled with 0.
		 * @param distance The distance in maximum metric.
		 * @param direction Direction of expansion.
		 **/
		void ExpandDirection(GGen_Distance distance, GGen_Direction direction);

		/**
		 * Fills all areas within a distance (in maximum metric) from any negative value with 0. The rest of the map will be filled with 1.
		 * @param distance The distance in maximum metric.
		 * @param direction Direction of shrinking.
		 **/
		void ShrinkDirection(GGen_Distance distance, GGen_Direction direction);

		/**
		 * Fills all areas within a distance (in maximum metric) from any value greater than 0 with 1. The rest of the map will be filled with 0.
		 * @param distance The distance in maximum metric.
		 **/
		void Expand(GGen_Distance distance);

		/**
		 * Fills all areas within a distance (in maximum metric) from any negative value with 0. The rest of the map will be filled with 1.
		 * @param distance The distance in maximum metric.
		 **/
		void Shrink(GGen_Distance distance);

		/**
		 * Draws a border (made of value 1) around an area matching a condition. The non-border areas are filled with 0.
		 * @param mode Condition operator.
		 * @param threshold Condition value.
		 * @param outlineMode Inside or outside border.
		 **/
		void Outline(GGen_Comparison_Mode mode, GGen_Height threshold, GGen_Outline_Mode outlineMode);

		/**
		 * Replaces values in the array with information about steepness of slope (change in value) in that particular value.
		 * @param radius Convexity measurement radius (how big a terrain feature must be to show on the convexity map).
		 **/
		void ConvexityMap(GGen_Distance radius);

		/**
		 * Applies a turbulence distortion filter on the height map.
		 * @param waveLength Size of one distortion wave.
		 * @param amplitude Strength of the distortion effect.
		 **/
		void Distort(GGen_Size waveLength, GGen_Distance amplitude);

		/**
		 * Replaces all values with values representing horizontal angle of surface normal in the tile. Angle 0° (eastern slope) is represented by value 0, angles in range (0°, 180°) are represented by negative values in range (GGEN_MIN_HEIGHT, 0) and angles in range (180°, 360°) are represented by positive values in range (0, GGEN_MAX_HEIGHT).
		 * @note Flat areas (with normal pointing directly upward) are replaced with value GGEN_INVALID_HEIGHT. 
		 **/
		void NormalMap();

		/** 
		 * Replaces all values with values representing angle difference between surface normal in the tile and given angle. Angle difference of 0° is represented by value 0, angle difference of 180° is represented by GGEN_MAX_HEIGHT.
		 * @param angle The angle in degrees.
		 * @note Flat areas (with normal pointing directly upward) are replaced with value GGEN_INVALID_HEIGHT. 
		 **/
		void NormalDifferenceMap(int32 angle);

		/**
		 * Returns surface normal angle in a tile. Angle 0° (eastern slope) is represented by value 0, angles in range (0°, 180°) are represented by negative values in range (GGEN_MIN_HEIGHT, 0) and angles in range (180°, 360°) are represented by positive values in range (0, GGEN_MAX_HEIGHT).
		 * @param x X coordinate of the tile.
		 * @param y Y coordinate of the tile.
		 * @return Surface normal angle. 
		 **/
		GGen_Height GetNormal(GGen_Coord x, GGen_Coord y);

		void SimpleErosion(uint8 numRounds, uint8 erosionFactor, bool enableSedimentation);

		/**
		 * Generates a map representing flow of water of over the current height map.
		 * @param duration Duration of the simulation, dramatically increases time complexity.
		 * @param waterAmount Water amount multiplier in (0, 10) range. 
		 **/
		double FlowMap(double duration, double waterAmount);

		/**
		 * Applies thermal erosion onto the height map.
		 * @param duration Duration of the simulation, dramatically increases time complexity.
		 * @param talusAngle Angle towards which will all slopes be eroded.
		 **/
		void ThermalWeathering(double duration, double talusAngle);

		/**
		 * Applies hydraulic and thermal erosion onto the height map.
		 * @param duration Duration of the simulation, dramatically increases time complexity.
		 * @param thermalWeatheringAmount Thermal weathering effect multiplier.
		 * @param waterAmount Water amount multiplier in (0, 10) range. 
		 **/
		void Erosion(double duration, double thermalWeatheringAmount, double waterAmount);

		static void FreeAllInstances(){
			while(GGen_Data_2D::instances.begin() != GGen_Data_2D::instances.end()){
				delete (*GGen_Data_2D::instances.begin());
			}
		};
};

