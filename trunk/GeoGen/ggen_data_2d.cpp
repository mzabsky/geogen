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
	assert(x > 1 && y > 1);

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
	assert(x > 1 && y > 1);
	
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
 * @param y coordinate of the value
 * @param target width
 * @param target height
 */
int16 GGen_Data_2D::GetValue(uint16 x, uint16 y, uint16 scale_to_x, uint16 scale_to_y){
	// TODO: poresit polozky na zacatku a konci pole
	assert(y < scale_to_y && x < scale_to_x && scale_to_y > 0 && scale_to_x > 0);

	/* No interpolation needed if the sizes are equal */
	if(scale_to_x == this->x && scale_to_y == this->y) return data[x + this->x * y];

	int16 value_x, value_y;	
	
	double ratio_x = (double) (scale_to_x - 1) / (double) (this->x - 1);
	double ratio_y = (double) (scale_to_y - 1) / (double) (this->y - 1);

	/* How much does the source tile overlap over the smaller grid? */
	double remainder_x = (x / ratio_x) - floor(x / ratio_x);
	double remainder_y = (y / ratio_y) - floor(y / ratio_y);

	/* The grid anchor points */
	uint16 base_x = scale_to_x > this->x ? (uint16) floor((double)x / ratio_x) : (uint16) floor((double)x / ratio_x + 0.5);
	uint16 base_y = scale_to_y > this->y ? (uint16) floor((double)y / ratio_y) : (uint16) floor((double)y / ratio_y + 0.5);
	
	/* Calculate the interpolated value for horizontal axis */
	if(scale_to_x > this->x){
		value_x = (int16) ((double) data[(uint16) base_x + this->x * base_y] * (1 - remainder_x) + (double) data[(uint16) base_x + 1 + this->x * base_y] * (remainder_x));
	}
	else{
		value_x = (int16) data[(uint16) base_x + this->x * base_y];
	}

	// Calculate the interpolated value for vertical axis
	if(scale_to_y > this->y){
		value_y = (int16) ((double) data[(uint16) base_x + this->x * base_y] * (1 - remainder_y) + (double) data[base_x + this->x * (base_y + 1)] * (remainder_y));
	}
	else{
		value_y = (int16) data[(uint16) base_x + this->x * base_y];
	}

	// Return the arithmetic mean of the vertical and horizontal values
	return (value_x + value_y) / 2;
}

/** 
 * Sets value on coordinate
 * @param x coordinate
 * @param y coordinate
 * @param value to use
 */
void GGen_Data_2D::SetValue(uint16 x, uint16 y, int16 value){
	assert(x < this->x && y < this->y);
	
	data[x + this->x * y] = value;
}

/** 
 * Sets value in rect defined by coordinates of its edges. All edges are included into the filled area.
 * @param x coordinate of the left edge
 * @param y coordinate of the top edge
 * @param x coordinate of the right edge
 * @param y coordinate of the bottom edge
 * @param value to use
 */
void GGen_Data_2D::SetValueInRect(uint16 x1, uint16 y1, uint16 x2, uint16 y2, int16 value){
	assert(x2 < this->x && y2 < this->y);
	assert(x1 < x2 && y1 < y2);
	
	for(uint16 i = y1; i <= y2; i++){
		for(uint16 j = x1; j <= x2; j++){
			data[j + this->x * i] = value;
		}
	}
}

/**
 * Change size of the array 
 * @param width of the new array
 * @param height of the new array
 * @param scale_values - should the values be scaled too?
 */
void GGen_Data_2D::ScaleTo(uint16 new_x, uint16 new_y, bool scale_values){
	assert(new_x > 1 && new_y > 1);

	/* Pick the ratio for values as arithmetic average of horizontal and vertical ratios */
	double ratio = ((double) new_x / (double) x + (double) new_y / (double) y) / 2.0;

	/* Allocate the new array */
	int16* new_data = new int16[new_x * new_y];

	assert(new_data != NULL);

	/* Fill the new array */
	for(uint16 i = 0; i < new_y; i++){
		for(uint16 j = 0; j < new_x; j++){
			new_data[j + i * new_x] = scale_values ? (int16) ((double) GetValue(j , i, new_x, new_y) * ratio) : GetValue(j , i, new_x, new_y);
		}
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	x = new_x;
	y = new_y;
	length = new_x * new_y;
}

/**
 * Change size of the array 
 * @param ratio in scale 100% = 1, 0.5 = 50%, 2.0 = 200%
 * @param scale_values - should the values be scaled too?
 */
void GGen_Data_2D::Scale(double ratio, bool scale_values){
	ScaleTo((uint16) ((uint16) (double) x * ratio), (uint16) ((double) y * ratio), scale_values);
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


/** 
 * Combines the array with second array by just adding them together
 * @param addend to be combined with
 */
void GGen_Data_2D::Add(GGen_Data_2D* addend){
	/* Scale the addend as necessary */
	for(uint16 i = 0; i < y; i++) 
	{
		for(uint16 j = 0; j < x; j++)
		{
			data[j + i * x] += addend->GetValue(j, i , x, y);
		}
	}
}

/** 
 * Multiplies all the values in the array by a flat number
 * @param value to be used
 */
void GGen_Data_2D::Multiply(double factor){
	for(uint32 i = 0; i < length; i++) data[i] = (uint16) (factor * (double) data[i]);
}


/** 
 * Multiplies current array by the factor
 * @param factor to be combined with
 */
void GGen_Data_2D::Multiply(GGen_Data_2D* factor){
	/* Scale the factor as necessary */
	for(uint16 i = 0; i < y; i++) 
	{
		for(uint16 j = 0; j < x; j++)
		{
			data[j + i * x] *= factor->GetValue(j, i , x, y);
		}
	}
}

/** 
 * Multiplies all the values in the array by a flat number
 * @param value to be used
 */
void GGen_Data_2D::Invert(){
	for(uint32 i = 0; i < length; i++) data[i] = - data[i];
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

/*
 * Unifies the data with data from the other array. The other array is scaled to fit the object.
 * Negative data are treated the same as positive - the higher value remains.
 * @param the victim
 */
void GGen_Data_2D::Union(GGen_Data_2D* unifiee){
	for(uint16 i = 0; i < y; i++){
		for(uint16 j = 0; j < x; j++){	
			data[j + i * x] = MIN(data[j + i * x], unifiee->GetValue(j, i, x, y));
		}
	}
}

/*
 * Unifies the data with data from the other array. The other array is scaled to fit the object.
 * Negative data are treated the same as positive - the higher value remains.
 * @param the victim
 */
void GGen_Data_2D::Intersection(GGen_Data_2D* intersectee){
	for(uint16 i = 0; i < y; i++){
		for(uint16 j = 0; j < x; j++){	
			data[j + i * x] = MIN(data[j + i * x], intersectee->GetValue(j, i, x, y));
		}
	}
}

/*
 * Projects 1D array onto a 2D array
 * @param profile to be projected
 * @param direction of projection
 */
void GGen_Data_2D::Project(GGen_Data_1D* profile, GGen_Direction direction){
	if(direction == GGEN_HORIZONTAL){
		for(uint16 i = 0; i < y; i++){
			for(uint16 j = 0; j < x; j++){		
				data[j + i * x] = profile->GetValue(i, y);
			}
		}
	}
	else{
		for(uint16 i = 0; i < y; i++){
			for(uint16 j = 0; j < x; j++){		
				data[j + i * x] = profile->GetValue(j, x);
			}
		}
	}
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
			int32 distance = (int32) sqrt((double) (cross_x*cross_x + cross_y*cross_y));
	
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