/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

class GGen_Data_2D{
	public:
		int16* data;
		uint16 x;
		uint16 y;
		uint32 length;

		/* Constructors/destructors */
		GGen_Data_2D(uint16 x, uint16 y);
		GGen_Data_2D(uint16 x, uint16 y, int16 value);
		GGen_Data_2D(GGen_Data_2D& victim);
		~GGen_Data_2D();

		/* Basic data I/O */
		void SetValue(uint16 x, uint16 y, int16 value);
		void SetValueInRect(uint16 x1, uint16 y1, uint16 x2, uint16 y2, int16 value);
		int16 GetValue(uint16 x, uint16 y);
		int16 GetValue(uint16 x, uint16 y, uint16 scale_to_x, uint16 scale_to_y);

		/* Elementary data manipulation */
		void Add(int16 value);
		void Add(GGen_Data_2D* addend);
		void AddTo(int16 offset_x, int16 offset_y, GGen_Data_2D* addend);
		void AddMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative);
		void AddMasked(int16 value, GGen_Data_2D* mask, bool relative);
		void Multiply(double factor);
		void Multiply(GGen_Data_2D* factor);
		void Invert();
		void Scale(double ratio, bool scale_values);
		void ScaleTo(uint16 new_x, uint16 new_y, bool scale_values);
		void ScaleValuesTo(int16 new_min, int16 new_max);
		void ResizeCanvas(int16 new_x, int16 new_y, int16 new_zero_x, int16 new_zero_y);
		void Fill(int16 value);
		void Clamp(int16 min, int16 max);
		int16 Min();
		int16 Max();
		void Rotate(GGen_Angle angle);
		void Flip(GGen_Direction direction);
		void Intersection(GGen_Data_2D* intersectee);
		void Union(GGen_Data_2D* unifiee);

		void Shift(GGen_Data_1D* profile, GGen_Direction direction, GGen_Overflow_Mode mode);
		void Project(GGen_Data_1D* profile, GGen_Direction direction);

		/* Advanced data manipulation */
		void Gradient(uint16 from_x, uint16 from_y, uint16 to_x, uint16 to_y, int16 from_value, int16 to_value, bool fill_outside);
		void Gradient(uint16 from_x, uint16 from_y, uint16 to_x, uint16 to_y, GGen_Data_1D* pattern, bool fill_outside);
		void RadialGradient(uint16 center_x, uint16 center_y, uint16 radius, int16 min, int16 max, bool fill_outside);
		void RadialGradient(uint16 center_x, uint16 center_y, uint16 radius, GGen_Data_1D* pattern, bool fill_outside);
		void Noise(uint16 min_feature_size, uint16 max_feature_size, GGen_Amplitudes* amplitudes);
		void Noise(uint16 min_feature_size, uint16 max_feature_size);
		void Smooth(uint8 radius, GGen_Direction direction);
		void Smooth(uint8 radius);
		void Flood(double water_amount);

		/* Human interface functions */
		void Window(bool fixed);
};

