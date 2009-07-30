/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
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

#include "sqplus.h"


#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen.h"

extern GGen_Amplitudes* ggen_std_noise;
extern GGen* ggen_current_object;

/** 
 * Creates a 1D data array and fills it with zeros
 * @param length of the array
 */
GGen_Data_1D::GGen_Data_1D(uint16 length){
	GGen_Script_Assert(length > 1);

	/* Allocate the array */
	data = new int16[length];

	GGen_Script_Assert(data != NULL);

	this->length = length;

	Fill(0);
}

/** 
 * Creates a 1D data array and fills it with a constant value
 * @param length of the array
 * @param value to be filled with
 */
GGen_Data_1D::GGen_Data_1D(uint16 length, int16 value){
	GGen_Script_Assert(length > 1);

	/* Allocate the array */
	data = new int16[length];

	GGen_Script_Assert(data != NULL);

	this->length = length;

	Fill(value);
}

/*
 * Copy constructor
 * @param victim to be cloned
 */
GGen_Data_1D::GGen_Data_1D(GGen_Data_1D& victim){
	/* Allocate the array */
	data = new int16[victim.length];

	GGen_Script_Assert(data != NULL);
	GGen_Script_Assert(victim.data != NULL);

	/* Copy the data */
	memcpy(data, victim.data, sizeof int16 * victim.length);
	length = victim.length;
}

GGen_Data_1D::~GGen_Data_1D(){
	delete [] data;
}

/** 
 * Reads and returns one value from the array
 * @param x coordinate of the value
 */
int16 GGen_Data_1D::GetValue(uint16 x){
	GGen_Script_Assert(x < length);
	
	return data[x];
}

/** 
 * Reads and returns one value from the array as if its size was scale_to_x
 * @param x coordinate of the value
 * @param target length of the array
 */
int16 GGen_Data_1D::GetValue(uint16 x, uint16 scale_to_x){
	// TODO: poresit polozky na zacatku a konci pole
	//GGen_Script_Assert(x < length || x < scale_to_x);
	if(!(x < length || x < scale_to_x)){
		cout << "ha";
	}

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
 * Sets value on coordinate
 * @param coordinate to modify
 * @param value to use
 */
void GGen_Data_1D::SetValue(uint16 x, int16 value){
	GGen_Script_Assert(x < length);

	data[x] = value;
}

void GGen_Data_1D::SetValueInRange(uint16 from, uint16 to, int16 value){
	GGen_Script_Assert(from < length || to < length);

	for(uint16 i = from; i <= to; i++) data[i] = value;
}

/** 
 * Fills the array with value
 * @param value to be used
 */
void GGen_Data_1D::Fill(int16 value){
	for(uint16 i = 0; i < length; i++) data[i] = value;
}

/** 
 * Adds a flat value to each value in the array
 * @param value to be used
 */
void GGen_Data_1D::Add(int16 value){
	for(uint16 i = 0; i < length; i++) data[i] += value;
}

/** 
 * Combines the array with second array by just adding them together
 * @param addend to be combined with
 */
void GGen_Data_1D::Add(GGen_Data_1D* addend){
	/* Scale the addend as necessary */
	for(uint16 i = 0; i < length; i++) data[i] += addend->GetValue(i, length);
}

/*
 * Adds values from (unscaled) addend to the array
 * @param offset of the addend coords
 * @param addend - the second array
 */
void GGen_Data_1D::AddTo(int16 offset, GGen_Data_1D* addend){
	
	/* Walk through the items where the array and the addend with ofset intersect */
	for(uint16 i = MAX(0, offset); i < MIN(length, offset + addend->length); i++){
		data[i] += addend->data[i - offset];
	}
}

void GGen_Data_1D::AddMasked(GGen_Data_1D* addend, GGen_Data_1D* mask, bool relative){
	int16 min = 0;
	int16 max = 255;

	if(relative){
		min = this->Min();
		max = this->Max() - min;
	}

	for(uint16 i = 0; i < length; i++) 
	{
		data[i] += addend->GetValue(i, length) * (mask->GetValue(i, length) - min) / max;
	}
}

void GGen_Data_1D::AddMasked(int value, GGen_Data_1D* mask, bool relative){
	int16 min = 0;
	int16 max = 255;

	if(relative){
		min = this->Min();
		max = this->Max() - min;
	}

	for(uint16 i = 0; i < length; i++) 
	{
		data[i] += value * (mask->GetValue(i, length) - min) / max;
	}
}

/** 
 * Scales each value in the array by a number. 100% = 1, 0.5 = 50%, 2.0 = 200%
 * @param value to be used
 */
void GGen_Data_1D::Multiply(double value){
	for(uint16 i = 0; i < length; i++) data[i] = (int16) ((double) data[i] * value);
}

/** 
 * Scales each value in the array by a value from the second array. 100% = 1, 0.5 = 50%, 2.0 = 200%
 * @param value to be used
 */
void GGen_Data_1D::Multiply(GGen_Data_1D* factor){
	for(uint16 i = 0; i < length; i++) data[i] = (int16) ((double) data[i] * factor->GetValue(i, length));
}

/** 
 * Inverts signs of all values in the array
 */
void GGen_Data_1D::Invert(){
	for(uint16 i = 0; i < length; i++) data[i] = -data[i];
}

/**
 * Change size of the array 
 * @param length of the new array
 * @param scale_values - should the values be scaled too?
 */
void GGen_Data_1D::ScaleTo(uint16 new_length, bool scale_values){
	GGen_Script_Assert(new_length > 0);

	double ratio = new_length / length;

	/* Allocate the new array */
	int16* new_data = new int16[new_length];

	GGen_Script_Assert(new_data != NULL);

	/* Fill the new array */
	for(uint16 i = 0; i < new_length; i++){
		new_data[i] = scale_values ? (int16) ((double) GetValue(i, new_length) * ratio) : GetValue(i, new_length);
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	length = new_length;
}

/**
 * Scales the values so they all fit to the passed range
 * @param new minimum value
 * @param new maximum value
 */
void GGen_Data_1D::ScaleValuesTo(int16 new_min, int16 new_max)
{
	GGen_Script_Assert(new_max > new_min);

	int16 min = this->Min();
	int16 max = this->Max() - min;

	new_max -= new_min;

	for(uint16 i = 0; i < length; i++){
		data[i] = new_min + (data[i] - min) * new_max / max;
	}
}

/**
 * Change size of the array 
 * @param ratio in scale 100% = 1, 0.5 = 50%, 2.0 = 200%
 * @param scale_values - should the values be scaled too?
 */
void GGen_Data_1D::Scale(double ratio, bool scale_values){
	ScaleTo((uint16) ((double) length * ratio), scale_values);
}

/*
 * Crops all indices outside given interval from the array. The border points will
 * remain in the area.
 * @param minimum index of the range
 * @param maximum index of the range
 */
void GGen_Data_1D::ResizeCanvas(int16 new_length, int16 new_zero){
	/* Allocate the new array */
	int16* new_data = new int16[new_length];

	GGen_Script_Assert(new_data != NULL);

	for(uint16 i = 0; i < new_length; i++){
		if(i + new_zero >= 0 && i + new_zero < length){
			new_data[i] = data[i + new_zero];
		}
		else new_data[i] = 0;
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	length = new_length;
}

/*
 * Clamps all values in the array to certain range
 * @param minimum value of the range
 * @param maximum value of the range
 */
void GGen_Data_1D::Clamp(int16 min, int16 max){
	GGen_Script_Assert(max > min);

	for(uint16 i = 0; i < length; i++){
		if(data[i] > max) data[i] = max;
		else if(data[i] < min) data[i] = min;
	}
}

/*
 * Inverts order of items in the array
 */
void GGen_Data_1D::Flip(){
	int16 temp;
	
	/* Go through the first half of the array and flip the value with its counterpart indexed from end */
	for(uint16 i = 0; i < length / 2; i++){
		temp = data[i];
		data[i] = data[length - 1 - i];
		data[length - 1 - i] = temp;
	}
}

/*
 * Returns the lowest value in the array
 */
int16 GGen_Data_1D::Min(){
	int16 temp = GGEN_MAX_HEIGHT;

	for(uint16 i = 0; i < length; i++){
		temp = temp > data[i] ? data[i] : temp;
	}

	return temp;
}

/*
 * Returns the highest value in the array
 */
int16 GGen_Data_1D::Max(){
	int16 temp = GGEN_MIN_HEIGHT;

	for(uint16 i = 0; i < length; i++){
		temp = temp < data[i] ? data[i] : temp;
	}

	return temp;
}

/*
 * Shifts all indices in the array left (if the distance is < 0) or right (otherwise)
 * @param difference of original and shifted indices
 * @param shift mode
 */
void GGen_Data_1D::Shift(int16 distance, GGen_Overflow_Mode mode){
	GGen_Script_Assert(distance < length && distance != 0 && distance > -length);
	
	/* Cycle mode */
	if(mode == GGEN_CYCLE){
		/* Allocate the new array */
		int16* new_data = new int16[length];

		GGen_Script_Assert(new_data != NULL);

		/* Fill the new array with shifted data */
		for(uint16 i = 0; i < length; i++){
			/* Some values can be just plainly shifted */
			if((distance > 0 && i < length - distance) || (distance < 0 && (signed) i >= -distance)){
				new_data[i + distance] = data[i];
			}
			/* Some must go through the right "border" */
			else if(distance > 0){
				new_data[i - length + distance] = data[i];
			}
			/* And some must go through the left "border" */
			else{
				new_data[i + length + distance] = data[i];
			}
		}

		/* Relink and delete the original array data */
		delete [] data;
		data = new_data;
	}
	/* Discard and Discard&fill mode */
	else{
		int16 temp;
		
		/* positive distance -> shift right*/
		if(distance > 0){
			for(int16 i = length - 1; i >= 0; i--){
				/* Some values can be just plainly shifted... */
				if(i > distance - 1){
					data[i] = data[i - distance];
					if(mode == GGEN_DISCARD_AND_FILL) temp = data[i];
				}
				/* And some must be filled with zeros / closest value */
				else{
					if(mode == GGEN_DISCARD_AND_FILL) data[i] = temp;
					else data[i] = 0;
				}
			}	
		}
		/* Negative distance -> shift left */
		else{
			for(int16 i = 0; i < length; i++){
				/* Some values can be just plainly shifted... */
				if(i < length + distance){
					data[i] = data[i - distance];
					if(mode == GGEN_DISCARD_AND_FILL) temp = data[i];
				}
				/* And some must be filled with zeros / closest value */
				else{
					if(mode == GGEN_DISCARD_AND_FILL) data[i] = temp;
					else data[i] = 0;
				}
			}				
		}
	}
}

/*
 * Unifies the data with data from the other array. The other array is scaled to fit the object.
 * Negative data are treated the same as positive - the higher value remains.
 * @param the victim
 */
void GGen_Data_1D::Union(GGen_Data_1D* unifiee){
	for(uint16 i = 0; i < length; i++) data[i] = MIN(data[i], unifiee->GetValue(i, length));
}

/*
 * Unifies the data with data from the other array. The other array is scaled to fit the object.
 * Negative data are treated the same as positive - the higher value remains.
 * @param the victim
 */
void GGen_Data_1D::Intersection(GGen_Data_1D* intersectee){
	for(uint16 i = 0; i < length; i++) data[i] = MAX(data[i], intersectee->GetValue(i, length));
}

/*
 * Reduces all data in the array to only two values. Values above treshold will be ones, values below
 * (treshold included) will be zeros.
 * @param treshold - maximum zero value
 */
void GGen_Data_1D::Monochrome(int16 treshold){
	for(uint16 i = 0; i < length; i++){
		data[i] = data[i] > treshold ? 1 : 0;
	}	
}

/*
 * Replaces the array  data with information about steepness of slopes at every point but borders
 */
void GGen_Data_1D::SlopeMap(){

	/* Allocate the new array */
	int16* new_data = new int16[length];

	GGen_Script_Assert(new_data != NULL);

	/* Calculate the slopes */
	for(uint16 i = 1; i < length - 1; i++){
		new_data[i] = abs(data[i - 1] - data[i + 1]);
	}

	new_data[0] = new_data[1];
	new_data[length-1] = new_data[length-2];

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;	
}

/*
 * Reduces all slopes in the array to max. 45 degrees (=1:1)
 * @param switch if the substract from or add values to too steep slopes
 */
void GGen_Data_1D::Normalize(GGen_Normalization_Mode mode){
	/* Additive mode */
	if(mode == GGEN_ADDITIVE){
		/* Fix left-to-right "downhills" */
		for(uint16 i = 1; i < length; i++){
			if(data[i] < data[i - 1] - 1) data[i] = data[i - 1] - 1;
		}

		/* Fix right-to-left "downhills" */
		for(uint16 i = length - 2; i > 0; i--){
			if(data[i] < data[i + 1] - 1) data[i] = data[i + 1] - 1;
		}
	}
	/* Substractive mode */
	else{
		/* Fix left-to-right "uphills" */
		for(uint16 i = 1; i < length; i++){
			if(data[i] > data[i - 1] + 1) data[i] = data[i - 1] + 1;
		}

		/* Fix right-to-left "uphills" */
		for(uint16 i = length - 2; i > 0; i--){
			if(data[i] > data[i + 1] + 1) data[i] = data[i + 1] + 1;
		}
	}

	data[0] = data[1];
	data[length-1] = data[length-2];

}

/**
 * Creates a smooth gradient of values
 * @param from - where should the gradient start
 * @param to - where should it end
 * @param from which value will the gradient start
 * @param to - maximum value
 * @param fill_flat - should be the areas outside the gradient area be filled with min/max values?
 */
void GGen_Data_1D::Gradient(uint16 from, uint16 to, int16 from_value, int16 to_value, bool fill_flat){
	GGen_Script_Assert(from < length || to < length);
	
	/* Swap values if necessary */
	if(from > to){
		uint16 temp = to;
		to = from;
		from = temp;
		
		int16 temp2 = to_value;
		to_value = from_value;
		from_value = temp2;
	}

	int16 base = from_value;
	double offset = (double) (to_value - base);
	uint16 max_distance = to - from;
	
	for(uint16 i = 0; i < length; i++){
		/* Calculate current distance from "from" and "to" */
		double distance_from = (double) abs(i - from);
		double distance_to = (double) abs(i - to);

		if(distance_from > max_distance) data[i] = fill_flat ? to_value: data[i];
		else if(distance_to > max_distance) data[i] = fill_flat ? from_value : data[i];
		else{
			double ratio = distance_to / max_distance;
			data[i] = base + (int16) (ratio * offset);
		}
	}
}

void GGen_Data_1D::Noise(uint16 min_feature_size, uint16 max_feature_size, GGen_Amplitudes* amplitudes){

	GGen_Script_Assert(amplitudes != NULL);

	/* Check if feature sizes are powers of 2 */
	//GGen_Script_Assert(((min_feature_size - 1) & min_feature_size) == 0);
	//GGen_Script_Assert(((max_feature_size - 1) & max_feature_size) == 0);

	/*

	for(int wave_length = max_feature_size; wave_length >= min_feature_size; wave_length /= 2){
		uint8 frequency= GGen_log2(wave_length);
		uint16 amplitude = amplitudes->data[frequency];

		// Generate "new" values (where is nothing yet) 
		for(uint16 i = wave_length; i < length; i += 2*wave_length){
			int16 interpolated = (data[i - wave_length] + data[(i + wave_length) >= length ? (i + wave_length - length) : (i + wave_length)]) / 2;
			data[i] = interpolated + GGen_Random((int16) -amplitude, (int16) amplitude);
		}

		// Improve the "old" values 
		for(uint16 i = 0; i < length; i += 2*wave_length){
			data[i] += GGen_Random((int16) -amplitude, (int16) amplitude);
		}
	}

*/

	uint8 frequency = GGen_log2(max_feature_size);
	uint16 amplitude = amplitudes->data[frequency];

	int16* new_data = new int16[length];

	GGen_Script_Assert(new_data != NULL);

	this->Fill(0);

	for(uint16 wave_length = max_feature_size; wave_length >= 1; wave_length /= 2){
		frequency = GGen_log2(wave_length);
		amplitude = amplitudes->data[frequency];

		if(wave_length < min_feature_size) break;

		for(uint16 i = 0; i < length; i += wave_length){
				new_data[i] = GGen_Random<int>(-amplitude, amplitude);
		}

		if(wave_length > 1)
		for(uint16 i = 0; i < length; i++){
			if(i % wave_length == 0) continue;

			uint16 nearest = i - i % wave_length;

			uint16 next = i - i % wave_length + wave_length;

			next = next >= length ? 0 : next;

			double vertical = (1 - cos( (i % wave_length) * 3.1415927 / wave_length)) * .5;

			data[i] += ( (double) new_data[nearest] * (1 - vertical) + (double) new_data[next] * vertical);
		} 

		for(uint16 i = 0; i < length; i += wave_length){
			data[i] += new_data[i];
		}

	}

	delete [] new_data;

}

void GGen_Data_1D::Noise(uint16 min_feature_size, uint16 max_feature_size){
	Noise(min_feature_size, max_feature_size, ggen_std_noise);
}

/*
 * Smooths the values using the linear blur algorithm.
 * @param radius from which are the values caulculated into the mean
 * @power weight of individual values. The current cell has always weight 1024
 */
void GGen_Data_1D::Smooth(uint8 radius){
	GGen_Script_Assert(radius > 0 && radius < length);
	
	/* Allocate the new array */
	int16* new_data = new int16[length];

	GGen_Script_Assert(new_data != NULL);

	/* Calculate size of the filter window */
	uint16 window_size = radius * 2 + 1;

	/* Prefill the window with value of the left edge + n leftmost values (where n is radius) */
	int32 window_value = data[0] * radius;

	for(uint8 i = 0; i < radius; i++){
		window_value += data[i];
	}

	/* In every step shift the window one tile to the right  (= substract its leftmost cell and add
	value of rightmost + 1). i represents position of the central cell of the window. */
	for(uint16 i = 0; i < length; i++){
		/* If the window is approaching right border, use the rightmost value as fill. */
		if((signed)i - (signed)radius < 0){
			window_value += data[i + radius] - data[0];
		}
		else if(i + radius < length){
			window_value += data[i + radius] - data[i - radius];
		}
		else{
			window_value += data[length - 1] - data[i - radius];
		}

		/* Set the value of current tile to arithmetic average of window tiles. */
		new_data[i] = window_value / window_size;
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;	

	return;
}

/*
 * Shifts the array values so given percentage of it is under zero (zero represents the water level).
 * @param percentage of the map to be flooded
 */
void GGen_Data_1D::Flood(double water_amount){
	GGen_Script_Assert(water_amount < 1);

	uint16 target = (uint16) (water_amount * (double) length);
	
	uint16 last_amount = 0;

	int16 level = this->Min();
	int16 max = this->Max();

	/* Go through the array values from bottom up and try to find the best fit to target water amount */
	while(level < max){
		/* Calculate the amount of waters above current level */
		uint16 amount = 0;
		for(uint16 i = 0; i < length; i++) {
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