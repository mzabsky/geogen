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

class GGen_Data_2D{
	public:
		GGen_Height* data;
		GGen_Size width;
		GGen_Size height;
		GGen_TotalSize length;

		/* Constructors/destructors */
		GGen_Data_2D(GGen_Size width, GGen_Size height);
		GGen_Data_2D(GGen_Size width, GGen_Size height, GGen_Height value);
		GGen_Data_2D(GGen_Data_2D& victim);
		~GGen_Data_2D();

		/* Basic data I/O */
		GGen_Size GetWidth();
		GGen_Size GetHeight();
		GGen_Index GetLength();
		void SetValue(GGen_Coord x, GGen_Coord y, GGen_Height value);
		void SetValueInRect(GGen_Coord x1, GGen_Coord y1, GGen_Coord x2, GGen_Coord y2, GGen_Height value);
		GGen_Height GetValue(GGen_Coord x, GGen_Coord y);
		GGen_Height GetValue(GGen_Coord x, GGen_Coord y, GGen_Size scale_to_width, GGen_Size scale_to_height);

		/* Elementary data manipulation */
		void Add(GGen_Height value);
		void Add(GGen_Data_2D* addend);
		void AddTo(GGen_Data_2D* addend, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);
		void AddMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative);
		void AddMasked(GGen_Height value, GGen_Data_2D* mask, bool relative);
		void Multiply(double factor);
		void Multiply(GGen_Data_2D* factor);
		void Invert();
		void Scale(double ratio, bool scale_values);
		void ScaleTo(GGen_Size new_width, GGen_Size new_height, bool scale_values);
		void ScaleValuesTo(GGen_Height new_min, GGen_Height new_max);
		void ResizeCanvas(GGen_Size new_width, GGen_Size new_height, GGen_CoordOffset new_zero_x, GGen_CoordOffset new_zero_y);
		void Fill(GGen_Height value);
		void Clamp(GGen_Height min, GGen_Height max);
		GGen_Height Min();
		GGen_Height Max();
		void Intersection(GGen_Data_2D* victim);
		void IntersectionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);
		void Union(GGen_Data_2D* victim);
		void UnionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y);
		void Combine(GGen_Data_2D* victim, GGen_Data_2D* mask, bool relative);
		void ReplaceValue(GGen_Height needle, GGen_Height replace);
		void Abs();

		void Shift(GGen_Data_1D* profile, GGen_Direction direction, GGen_Overflow_Mode mode);
		void Project(GGen_Data_1D* profile, GGen_Direction direction);
		GGen_Data_1D* GetProfile(GGen_Direction direction, GGen_Coord coordinate);

		/* Advanced data manipulation */
		void Gradient(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Height from_value, GGen_Height to_value, bool fill_outside);
		void Gradient(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Data_1D* pattern, bool fill_outside);
		void RadialGradient(GGen_Coord center_x, GGen_Coord center_y, GGen_Coord radius, GGen_Height min, GGen_Height max, bool fill_outside);
		void RadialGradient(GGen_Coord center_x, GGen_Coord center_y, GGen_Distance radius, GGen_Data_1D* pattern, bool fill_outside);
		void Noise(GGen_Size min_feature_size, GGen_Size max_feature_size, GGen_Amplitudes* amplitudes);
		void Smooth(GGen_Distance radius, GGen_Direction direction);
		void Smooth(GGen_Distance radius);
		void Flood(double water_amount);
		void Pattern(GGen_Data_2D* pattern);
		void Monochrome(GGen_Height treshold);
		void SelectValue(GGen_Height value);
		void SlopeMap();
		void Scatter(bool relative);
		void TransformValues(GGen_Data_1D* profile, bool relative);
		void Normalize(GGen_Normalization_Mode mode);
		void Normalize(GGen_Direction direction, GGen_Normalization_Mode mode);
		
		/* Matrix operations */
		void Transform(double a11, double a12, double a21, double a22, bool preserve_size);
		void Rotate(int32 angle, bool preserve_size);
		void Shear(int32 horizontal_shear, int32 vertical_shear, bool preserve_size);
		void Flip(GGen_Direction direction);


		/* External communication functions */
		void ReturnAs(const SqPlus::sq_std_string &name);
};

