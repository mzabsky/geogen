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
		void Multiply(double factor);
		void Multiply(GGen_Data_2D* factor);
		void Invert();
		void Scale(double ratio, bool scale_values);
		void ScaleTo(uint16 new_x, uint16 new_y, bool scale_values);
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

		/* Human interface functions */
		void Window(bool fixed);
};

