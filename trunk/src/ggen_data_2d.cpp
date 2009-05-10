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

// system and SDL headers
#include <stdio.h>   
#include <stdlib.h> 
#include <string>
//#include <SDL/SDL.h> 
#include <math.h>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include "ggen_support.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"

extern GGen_Amplitudes* ggen_std_noise;

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
	data = new int16[victim.length];

	assert(data != NULL);
	assert(victim.data != NULL);

	/* Copy the data */
	memcpy(data, victim.data, sizeof int16 * victim.length);
	length = victim.length;
	x = victim.x;
	y = victim.y;
}

GGen_Data_2D::~GGen_Data_2D(){
	delete [] data;
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
 * Scales the values so they all fit to the passed range
 * @param new minimum value
 * @param new maximum value
 */
void GGen_Data_2D::ScaleValuesTo(int16 new_min, int16 new_max)
{
	assert(new_max > new_min);

	int16 min = this->Min();
	int16 max = this->Max() - min;

	new_max -= new_min;

	if(max == 0) return;

	for(uint32 i = 0; i < length; i++){
		data[i] = new_min + (data[i] - min) * new_max / max;
	}
}

/**
 * Change size of the array 
 * @param ratio in scale 100% = 1, 0.5 = 50%, 2.0 = 200%
 * @param scale_values - should the values be scaled too?
 */
void GGen_Data_2D::Scale(double ratio, bool scale_values){
	ScaleTo((uint16) ((uint16) (double) x * ratio), (uint16) ((double) y * ratio), scale_values);
}

void GGen_Data_2D::ResizeCanvas(int16 new_x, int16 new_y, int16 new_zero_x, int16 new_zero_y){
	/* Allocate the new array */
	int16* new_data = new int16[new_x * new_y];

	assert(new_data != NULL);

	for(uint16 i = 0; i < new_y; i++){
		for(uint16 j = 0; j < new_x; j++){
			if(i + new_zero_y >= 0 && i + new_zero_y < y && j + new_zero_x >= 0 && j + new_zero_x){
				new_data[j + i * new_x] = data[j + new_zero_x + (i + new_zero_y) * x];
			}
			else new_data[j + i * new_x] = 0;
		}
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	length = new_x * new_y;
	x = new_x;
	y = new_y;
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

/*
 * Adds values from (unscaled) addend to the array
 * @param x offset of the addend coords
 * @param y offset of the addend coords
 * @param addend - the second array
 */
void GGen_Data_2D::AddTo(int16 offset_x, int16 offset_y, GGen_Data_2D* addend){
	/* Walk through the items where the array and the addend with offset intersect */
	for(uint16 i = MAX(0, offset_y); i < MIN(y, offset_y + addend->y); i++){
		for(uint16 j = MAX(0, offset_x); j < MIN(x, offset_x + addend->x); j++){
			data[j + i * x] += addend->data[(j - offset_x) + (i - offset_y) * addend->x];
		}
	}
}

/*
 * Combines values from the current array and the addend by adding them together.
 * The weight of data from the addend depends on values in the mask.
 * @param x offset of the addend coords
 * @param y offset of the addend coords
 * @param addend - the second array
 */
void GGen_Data_2D::AddMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative){
	int16 min = 0;
	int16 max = 255;

	if(relative){
		min = this->Min();
		max = this->Max() - min;
	}

	for(uint16 i = 0; i < y; i++) 
	{
		for(uint16 j = 0; j < x; j++)
		{
			data[j + i * x] += addend->GetValue(j, i, x, y) * (mask->GetValue(j, i, x, y) - min) / max;
		}
	}
}

void GGen_Data_2D::AddMasked(int16 value, GGen_Data_2D* mask, bool relative){
	int16 min = 0;
	int16 max = 255;

	if(relative){
		min = this->Min();
		max = this->Max() - min;
	}

	for(uint16 i = 0; i < y; i++) 
	{
		for(uint16 j = 0; j < x; j++)
		{
			data[j + i * x] += value * (mask->GetValue(j, i, x, y) - min) / max;
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

/**
  * Rotates the array clock-wise
  * @param angle
  */
void GGen_Data_2D::Rotate(GGen_Angle angle){
	
	if(angle = GGEN_0) return;

	uint16 new_x, new_y;
	if(angle == GGEN_180){
		new_x = x;
		new_y = y;
	}
	else{
		new_x = y;
		new_y = x;		
	}
	
	/* Allocate the new array */
	int16* new_data = new int16[new_x * new_y];

	assert(new_data != NULL);	

	/* Fill the new array with rotated values */
	for(uint16 i = 0; i < new_y; i++) 
	{
		for(uint16 j = 0; j < new_x; j++)
		{
			switch (angle){
				case GGEN_90:
					new_data[j + i * new_x] = data[i + j * x];
					break;
				case GGEN_180:
					new_data[(new_x - j) + i * new_x] = data[j + i * x];
					break;
				case GGEN_270:
					new_data[j + (new_y - i) * new_x] = data[i + j * x];
					break;
			}
		}
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	x = new_x;
	y = new_y;
}

/**
  * Flips the heightmap 
  * @param direction on which the heightmap will be flipped
  */
void GGen_Data_2D::Flip(GGen_Direction direction){
	int16 temp;

	if(direction == GGEN_HORIZONTAL){	
		for(uint16 i = 0; i < y; i++) 
		{
			for(uint16 j = 0; j < x / 2; j++)
			{
				temp = data[j + i * x];
				data[j + i * x] = data[(x - 1 - j) + i * x];
				data[(x - 1 - j) + i * x] = temp;
			}
		}
		int i = 1+1;
	}
	else{
		for(uint16 i = 0; i < y / 2; i++) 
		{
			for(uint16 j = 0; j < x; j++)
			{
				temp = data[j + i * x];
				data[j + i * x] = data[j + (y - 1 - i) * x];
				data[j + (y - 1 - i) * x] = temp;
			}
		}
	}
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
 * Clamps all values in the array to certain range
 * @param minimum value of the range
 * @param maximum value of the range
 */
void GGen_Data_2D::Clamp(int16 min, int16 max){
	assert(max > min);

	for(uint32 i = 0; i < length; i++){
		if(data[i] > max) data[i] = max;
		else if(data[i] < min) data[i] = min;
	}
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

void GGen_Data_2D::Shift(GGen_Data_1D* profile, GGen_Direction direction, GGen_Overflow_Mode mode){
	/* Cycle mode */
	if(mode == GGEN_CYCLE){
		/* Allocate the new array */
		int16* new_data = new int16[length];

		assert(new_data != NULL);

		for(uint16 i = 0; i < y; i++){
			for(uint16 j = 0; j < x; j++){		
				if(direction == GGEN_VERTICAL){
					/* Some values can be just plainly shifted */
					int16 distance = profile->GetValue(j, x);

					/*if(i==576 && j == 88){
						int d = 55;
						d++;
					}*/

					/*if(j + (i + distance) * x == 543288){
						int b = 50;
						b+=1;
					}*/



					if((distance >= 0 && i < y - distance) || (distance <= 0 && (signed) i >= -distance)){
						new_data[j + (i + distance) * x] = data[j + i * x];
					}
					/* Some must go through the right "border" */
					else if(distance >= 0){
						new_data[j + (i - y + distance) * x] = data[j + i * x];
					}
					/* And some must go through the left "border" */
					else{
						new_data[j + (i + y + distance) * x] = data[j + i * x];
					}					
				}
			}
		}

		/* Fill the new array with shifted data */
	//	for(uint16 i = 0; i < length; i++){
			/* Some values can be just plainly shifted */
		//	if((distance > 0 && i < length - distance) || (distance < 0 && (signed) i >= -distance)){
		//		new_data[i + distance] = data[i];
//			}
			/* Some must go through the right "border" */
	//		else if(distance > 0){
		//		new_data[i - length + distance] = data[i];
	//		}
			/* And some must go through the left "border" */
	//		else{
	//			new_data[i + length + distance] = data[i];
	//		}
	//	}

		/* Relink and delete the original array data */
		delete [] data;
		data = new_data;
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

void GGen_Data_2D::Noise(uint16 min_feature_size, uint16 max_feature_size, GGen_Amplitudes* amplitudes){

	assert(amplitudes != NULL);

	/* Check if feature sizes are powers of 2 */
	assert(((min_feature_size - 1) & min_feature_size) == 0);
	assert(((max_feature_size - 1) & max_feature_size) == 0);

	uint8 frequency = log2(max_feature_size);
	uint16 amplitude = amplitudes->data[frequency];

	/* First round - generate base values */
	for(int i = 0; i < y; i += max_feature_size){
		for(int j = 0; j < x; j += max_feature_size){
			data[j + i * x] = Random((int16) -amplitude, (int16) amplitude);
		}		
	}

	
	for(int wave_length = max_feature_size / 2; wave_length >= 1; wave_length /= 2){
		frequency = log2(wave_length);
		amplitude = amplitudes->data[frequency];

		for(uint16 i = 0; i < y; i += wave_length * 2){
			for(uint16 j = 0; j < x; j += wave_length * 2){
				if(j < x - wave_length) data[j + wave_length + i * x] = ( 
					data[j + i * x] +
					(j < x - wave_length * 2 ? data[j + 2 * wave_length + i * x] : 0)
					) / 2 + (wave_length >= min_feature_size ? Random((int16) -amplitude, (int16) amplitude) : 0);
				if(i < y - wave_length) data[j + (i + wave_length) * x] = ( 
					data[j + i * x] +
					(i < y - wave_length * 2 ? data[j + (i + 2 * wave_length) * x] : 0)
					) / 2 + (wave_length >= min_feature_size ? Random((int16) -amplitude, (int16) amplitude) : 0);
				if(j < x - wave_length && i < y - wave_length) data[j + wave_length + (i + wave_length) * x] = ( 
					data[j + i * x] +
					(i < y - wave_length * 2 ? data[j + (i + 2 * wave_length) * x] : 0) +
					(j < x - wave_length * 2 ? data[j + 2 * wave_length + i * x] : 0) +
					((j < x - wave_length * 2 && i < y - wave_length * 2) ? data[j + 2 * wave_length + (i + 2 * wave_length) * x] : 0)
					) / 4 + (wave_length >= min_feature_size ? Random((int16) -amplitude, (int16) amplitude) : 0);
				if(wave_length >= min_feature_size) data[j + i * x] += Random((int16) -amplitude, (int16) amplitude);
			}
		}
	}

} 

void GGen_Data_2D::Noise(uint16 min_feature_size, uint16 max_feature_size){
	Noise(min_feature_size, max_feature_size, ggen_std_noise);
}

/*
 * Shifts the array values so given percentage of it is under zero (zero represents the water level).
 * @param percentage of the map to be flooded
 */
void GGen_Data_2D::Flood(double water_amount){
	assert(water_amount < 1);

	uint32 target = (uint32) (water_amount * (double) length);
	
	uint32 last_amount = 0;

	int16 level = this->Min();
	int16 max = this->Max();

	/* Go through the array values from bottom up and try to find the best fit to target water amount */
	while(level < max){
		/* Calculate the amount of waters above current level */
		uint32 amount = 0;
		for(uint32 i = 0; i < length; i++) {
			if(data[i] >= level) amount++;
		}

		/* Is current level higher than the target? */
		if(amount <= target){
			/* Find if this level fits better than the previous (the closest fit applies) */
			if(amount - target < target - last_amount) break;
			else{
				level--;
				break;
			}
		}

		last_amount = amount;

		level++;
	}

	/* Shift the heights so given portion of the array is under zero */
	this->Add(-level);
}

void GGen_Data_2D::Smooth(uint8 radius){
	Smooth(radius, GGEN_HORIZONTAL);
	Smooth(radius, GGEN_VERTICAL);
}

void GGen_Data_2D::Smooth(uint8 radius, GGen_Direction direction){
	assert(radius > 0 && radius < x && radius < y);
	
	/* Allocate the new array */
	int16* new_data = new int16[length];

	assert(new_data != NULL);

	/* Calculate size of the filter window */
	uint16 window_size = radius * 2 + 1;

	if(direction == GGEN_HORIZONTAL){
		for(uint16 i = 0; i < y; i++){
			/* Prefill the window with value of the left edge + n leftmost values (where n is radius) */
			int32 window_value = data[x * i] * radius;

			for(uint8 j = 0; j < radius; j++){
				window_value += data[j + x * i];
			}

			/* In every step shift the window one tile to the right  (= substract its leftmost cell and add
			value of rightmost + 1). i represents position of the central cell of the window. */
			for(uint16 j = 0; j < x; j++){
				/* If the window is approaching right border, use the rightmost value as fill. */
				if((signed)j - (signed)radius < 0){
					window_value += data[j + radius + x * i] - data[x * i];
				}
				else if(j + radius < x){
					window_value += data[j + radius + x * i] - data[j - radius + x * i];
				}
				else{
					window_value += data[x - 1 + x * i] - data[j - radius + x * i];
				}

				/* Set the value of current tile to arithmetic average of window tiles. */
				new_data[j + x * i] = window_value / window_size;
			}
		}
	}
	else{ // vertical

		for(uint16 i = 0; i < x; i++){
			/* Prefill the window with value of the left edge + n topmost values (where n is radius) */
			int32 window_value = data[i] * radius;

			for(uint8 j = 0; j < radius; j++){
				window_value += data[i + j * x];
			}

			/* In every step shift the window one tile to the bottom  (= substract its topmost cell and add
			value of bottommost + 1). i represents position of the central cell of the window. */
			for(uint16 j = 0; j < y; j++){
				/* If the window is approaching right border, use the rightmost value as fill. */
				if((signed)j - (signed)radius < 0){
					window_value += data[i + (j + radius) * x] - data[i];
				}
				else if(j + radius < y){
					window_value += data[i + (j + radius) * x] - data[i + (j - radius) * x];
				}
				else{
					window_value += data[i + (y - 1) * x] - data[i + (j - radius) * x];
				}

				/* Set the value of current tile to arithmetic average of window tiles. */
				new_data[i + x * j] = window_value / window_size;
			}
		}
	}
	

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;	

	return;	
}