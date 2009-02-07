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
		//void SetValueInRange(uint16 from, uint16 to, int16 value);
		int16 GetValue(uint16 x, uint16 y);
		int16 GetValue(uint16 x, uint16 y, uint16 scale_to_x, uint16 scale_to_y);

		void Add(int16 value);
		void Fill(int16 value);
		int16 Min();
		int16 Max();

		/* Advanced data manipulation */
		void Gradient(uint16 from_x, uint16 from_y, uint16 to_x, uint16 to_y, int16 from_value, int16 to_value, bool fill_outside);
		void GGen_Data_2D::Gradient(uint16 from_x, uint16 from_y, uint16 to_x, uint16 to_y, GGen_Data_1D* pattern, bool fill_outside);
		void RadialGradient(uint16 center_x, uint16 center_y, uint16 radius, int16 min, int16 max, bool fill_outside);
		void RadialGradient(uint16 center_x, uint16 center_y, uint16 radius, GGen_Data_1D* pattern, bool fill_outside);

		/* Human interface functions */
		void Window(bool fixed);
};

