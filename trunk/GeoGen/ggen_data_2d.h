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
};