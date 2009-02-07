// system and SDL headers
#include <stdio.h>   
#include <stdlib.h> 
#include <string>
#include <SDL/SDL.h> 
#include <math.h>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include "ggen_support.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"

/** 
 * Creates a 2D data array and fills it with zeros
 * @param length of the array
 */
GGen_Data_2D::GGen_Data_2D(uint16 x, uint16 y){
	length = x * y;
	this->x = x;
	this->y = y;

	/* Allocate the array */
	data = new int16[length];

	assert(data != NULL);

	Fill(0);
}

/** 
 * Creates a 1D data array and fills it with a constant value
 * @param width of the array
 * @param height of the array
 * @param value to be filled with
 */
GGen_Data_2D::GGen_Data_2D(uint16 x, uint16 y, int16 value){
	length = x * y;
	this->x = x;
	this->y = y;

	/* Allocate the array */
	data = new int16[length];

	assert(data != NULL);

	Fill(value);
}

/*
 * Copy constructor
 * @param victim to be cloned
 */
GGen_Data_2D::GGen_Data_2D(GGen_Data_2D& victim){
	/* Allocate the array */
	data = new int16[length];

	assert(data != NULL);
	assert(victim.data != NULL);

	/* Copy the data */
	memcpy(data, victim.data, sizeof int16 * victim.length);
	length = victim.length;
	x = victim.x;
	y = victim.y;
}

/** 
 * Reads and returns one value from the array
 * @param x coordinate of the value
 * @param y coordinate of the value
 */
int16 GGen_Data_2D::GetValue(uint16 x, uint16 y){
	assert(x < this->x && y < this->y);
	
	return data[x + this->x * y];
}

/** 
 * Reads and returns one value from the array as if its size was scale_to_x * scale_to_y
 * @param x coordinate of the value
 */
int16 GGen_Data_2D::GetValue(uint16 x, uint16 y, uint16 scale_to_x, uint16 scale_to_y){
	// TODO: poresit polozky na zacatku a konci pole
	assert(x < length || x < scale_to_x);
	
	/* No interpolation needed if the sizes are equal */
	if(scale_to_x == length) return data[x];
	
	/* The target scale is larger, interpolation is necessary */
	else if(scale_to_x > length){
		double ratio = (double) (scale_to_x - 1) / (double) (length - 1);

		/* How much does the source tile overlap over the smaller grid? */
		double remainder = (x / ratio) - floor(x / ratio);

		/* Interpolate the value from two sorrounding values */
		return (int16) ((double) data[(uint16) floor(x / ratio)] * (1 - remainder) + (double) data[(uint16) floor(x / ratio) + 1] * (remainder));
	}

	/* The target is smaller, pick the closest value */
	else{
		double ratio = (double) (scale_to_x - 1) / (double) (length - 1);

		return (int16) data[(uint16) floor((double)x / ratio + 0.5)];
	}
}







/** 
 * Fills the array with value
 * @param value to be used
 */
void GGen_Data_2D::Fill(int16 value){
	for(uint32 i = 0; i < length; i++) data[i] = value;
}

/** 
 * Adds a flat value to each value in the array
 * @param value to be used
 */
void GGen_Data_2D::Add(int16 value){
	for(uint32 i = 0; i < length; i++) data[i] += value;
}

/*
 * Returns the lowest value in the array
 */
int16 GGen_Data_2D::Min(){
	int16 temp = GGEN_MAX_HEIGHT;

	for(uint32 i = 0; i < length; i++){
		temp = temp > data[i] ? data[i] : temp;
	}

	return temp;
}

/*
 * Returns the highest value in the array
 */
int16 GGen_Data_2D::Max(){
	int16 temp = GGEN_MIN_HEIGHT;

	for(uint32 i = 0; i < length; i++){
		temp = temp < data[i] ? data[i] : temp;
	}

	return temp;
}




void GGen_Data_2D::Gradient(uint16 from_x, uint16 from_y, uint16 to_x, uint16 to_y, GGen_Data_1D* pattern, bool fill_outside){
	int32 target_x = to_x - from_x;
	int32 target_y = to_y - from_y;

	/* Width of the gradient strip */
	double max_dist = sqrt((double) (abs(to_x - from_x) * abs(to_x - from_x) + abs(to_y - from_y) * abs(to_y - from_y)));

	for(uint16 i = 0; i < y; i++){
		for(uint16 j = 0; j < x; j++){
			int32 point_x = j - from_x;
			int32 point_y = i - from_y;

			/* Get the point on the gradient vector to which is the current point closest */
			int32 cross_x = (target_x * (target_x * point_x + target_y * point_y)) / (target_x * target_x + target_y * target_y);
			int32 cross_y = (target_y * (target_x * point_x + target_y * point_y)) / (target_x * target_x + target_y * target_y);

			/* Calculate the distance from the "from" pont */
			int32 distance = sqrt((double) (cross_x*cross_x + cross_y*cross_y));
	
			/* Fill/skip the outside areas */
			if ((cross_x < 0 == (signed)to_x-(signed)from_x > 0) || (cross_y < 0 == (signed)to_y-(signed)from_y > 0) || distance >= floor(max_dist)){
				if(fill_outside){
					data[j + x * i] = ((cross_x < 0 == (signed)to_x-(signed)from_x >= 0) || (cross_y < 0 == (signed)to_y-(signed)from_y > 0))  ? pattern->GetValue(0) : pattern->GetValue(pattern->length - 1);
				}
				continue;
			}

			/* Apply it to the array data */
			data[j + x * i] = pattern->GetValue((uint16) distance, (uint16) max_dist);
		}
	}
}

void GGen_Data_2D::Gradient(uint16 from_x, uint16 from_y, uint16 to_x, uint16 to_y, int16 from_value, int16 to_value, bool fill_outside){
	GGen_Data_1D temp(2);
	temp.SetValue(0, from_value);
	temp.SetValue(1, to_value);

	Gradient(from_x, from_y, to_x, to_y, &temp, fill_outside);
}

void GGen_Data_2D::RadialGradient(uint16 center_x, uint16 center_y, uint16 radius, GGen_Data_1D* pattern, bool fill_outside){
	assert(radius > 0 && pattern != NULL);

	for(uint16 i = 0; i < y; i++){
		for(uint16 j = 0; j < x; j++){
			uint16 distance = (uint16) sqrt((double) (abs(j - center_x) * abs(j - center_x) + abs(i - center_y) * abs(i - center_y)));
		 
			if(distance < radius) data[j + x * i] = pattern->GetValue(distance, radius);
			else if (fill_outside) data[j + x * i] = pattern->GetValue(pattern->length - 1);
		}
	}
}

void GGen_Data_2D::RadialGradient(uint16 center_x, uint16 center_y, uint16 radius, int16 min, int16 max, bool fill_outside){
	assert(radius > 0);
	
	max = max - min;

	for(uint16 i = 0; i < y; i++){
		for(uint16 j = 0; j < x; j++){
			uint16 distance = (uint16) sqrt((double) (abs(j - center_x) * abs(j - center_x) + abs(i - center_y) * abs(i - center_y)));

			if(distance < radius)  data[j + x * i] = min + max * distance / radius;
			else if(fill_outside) data[j + x * i] = min + max;
		}
	}
}