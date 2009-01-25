// hide stupid "sprintf is deprecated function, use our better alternative" MSVS warnings
#define _CRT_SECURE_NO_WARNINGS

// system and SDL headers
#include <stdio.h>   
#include <stdlib.h> 
#include <string>
#include <SDL/SDL.h> 
#include <SDL/SDL_ttf.h> 
#include <math.h>
#include <time.h> 
#include <SDL/SDL_gfxPrimitives.h>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <assert.h>
using namespace std;

typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

#define MAX(a,b) (a > b? a: b) 
#define MIN(a,b) (a < b? a: b) 

#define GGEN_INVALID_HEIGHT -32768
#define GGEN_MIN_HEIGHT -32767
#define GGEN_MAX_HEIGHT 32767

enum GGen_Normalization_Mode{
	GGEN_ADDITIVE,
	GGEN_SUBSTRACTIVE
};

template <class T>
T Random(T min, T max){
	double random = (double)rand() / (double) RAND_MAX;
	return (T) (min + random * (max - min));
}

class GGen_Data_1D{
	public:
		int16* data;
		uint16 length;

		GGen_Data_1D(uint16 length);
		GGen_Data_1D(uint16 length, int16 value);
		~GGen_Data_1D();

		/* Basic data I/O */
		void SetValue(uint16 x, int16 value);
		void SetValueInRange(uint16 from, uint16 to, int16 value);
		int16 GetValue(uint16 x);
		int16 GetValue(uint16 x, uint16 scale_to_x);

		/* Elementary artihmetic and logic operations */
		void Add(int16 value);
		void Add(GGen_Data_1D* addend);
		void Multiply(double value);
		void Multiply(GGen_Data_1D* factor);
		void Invert();
		void Scale(double ratio, bool scale_values);
		void ScaleTo(uint16 new_length, bool scale_values);
		void Fill(int16 value);
		void Crop(uint16 left_bound, uint16 right_bound);
		void Clamp(int16 min, int16 max);
		void Flip();

		/* Advanced operations with array data */
		void Normalize(GGen_Normalization_Mode mode);
		void Gradient(uint16 from, uint16 to, int16 from_value, int16 to_value, bool fill_flat);
		void Noise(int16 min_value, int16 max_value, uint16 num_octaves, int16 octave_skip, uint16* octaves);

		/* Human interface functions */
		void Print();
};

/** 
 * Creates a 1D data array and fills it with zeros
 * @param length of the array
 */
GGen_Data_1D::GGen_Data_1D(uint16 length){
	/* Allocate the array */
	data = new int16[length];

	assert(data != NULL);

	this->length = length;

	Fill(0);
}

/** 
 * Creates a 1D data array and fills it with value
 * @param length of the array
 * @param value to be filled with
 */
GGen_Data_1D::GGen_Data_1D(uint16 length, int16 value){
	/* Allocate the array */
	data = new int16[length];

	assert(data != NULL);

	this->length = length;

	Fill(value);
}

GGen_Data_1D::~GGen_Data_1D(){
	delete [] data;
}

/** 
 * Reads and returns one value from the array
 * @param x coordinate of the value
 */
int16 GGen_Data_1D::GetValue(uint16 x){
	assert(x < length);
	
	return data[x];
}

/** 
 * Reads and returns one value from the array as if its size was scale_to_x
 * @param x coordinate of the value
 */
int16 GGen_Data_1D::GetValue(uint16 x, uint16 scale_to_x){
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
 * Sets value on coordinate
 * @param coordinate to modify
 * @param value to use
 */
void GGen_Data_1D::SetValue(uint16 x, int16 value){
	assert(x < length);

	data[x] = value;
}

void GGen_Data_1D::SetValueInRange(uint16 from, uint16 to, int16 value){
	assert(from < length || to < length);

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
 * Combines the array with second array by just adding the together
 * @param addend to be combined with
 */
void GGen_Data_1D::Add(GGen_Data_1D* addend){
	/* Scale the addend as necessary */
	for(uint16 i = 0; i < length; i++) data[i] += addend->GetValue(i, length);
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
	assert(new_length > 0);

	double ratio = new_length / length;

	/* Allocate the new array */
	int16* new_data = new int16[new_length];

	assert(new_data != NULL);

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
void GGen_Data_1D::Crop(uint16 left_bound, uint16 right_bound){
	assert(right_bound < length || left_bound > right_bound);

	/* Allocate the new array */
	int16* new_data = new int16[right_bound - left_bound + 1];

	assert(new_data != NULL);

	for(uint16 i = left_bound; i <= right_bound + 1; i++){
		new_data[i - left_bound] = data[i];
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	length = right_bound - left_bound + 1;
}

/*
 * Clamps all values in the array to certain range
 * @param minimum value of the range
 * @param maximum value of the range
 */
void GGen_Data_1D::Clamp(int16 min, int16 max){
	assert(max > min);

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
	assert(from < length || to < length);
	
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
			double ratio = distance_from / max_distance;
			data[i] = base + (int16) (ratio * offset);
		}
	}
}

void GGen_Data_1D::Noise(int16 min_value, int16 max_value, uint16 num_octaves, int16 octave_skip, uint16* octaves){
	/* Make sure length is a power of two + 1 */
	assert(((length - 1) & (length - 2)) == 0);

	assert(octaves != NULL);

	int amplitude = num_octaves + octave_skip;

	int16* temp = new int16[length];

	/* Clear the height array */
	for(uint16 i = 0; i < length; i++){
		data[i] = GGEN_INVALID_HEIGHT;
	}

	while(amplitude >= octave_skip){
		uint16 step_size = (uint16) pow(2, (double) amplitude);

		memcpy(temp, data, length * sizeof int16);

		/* Skip some steps if the length is too small */
		if(step_size > length){
			amplitude--;
			continue;
		}

		for(uint16 i = 0; i < length; i += step_size){
			/* Do not interpolate the value during the first step */
			if(step_size != length - 1 && data[i] == GGEN_INVALID_HEIGHT){
				data[i] = (temp[i - step_size] + temp[i + step_size]) / 2;
				data[i] += Random((int16) -octaves[num_octaves - amplitude - 1], (int16) octaves[num_octaves - amplitude - 1]);
			}
			else if(step_size != length - 1){
				data[i] += Random((int16) -octaves[num_octaves - amplitude - 1], (int16) octaves[num_octaves - amplitude - 1]);
			}

			else data[i] += Random((int16) -octaves[num_octaves - amplitude - 1], (int16) octaves[num_octaves - amplitude - 1]);
			
		}

		//cout << "NOISE"<<amplitude << "=" <<(signed) step_size << " - " << octaves[num_octaves - amplitude - 1] << "\n";

		for(uint16 i = 0; i < length; i++){
			cout << data[i]<<",";
		}

		cout << "dsf\n";

		amplitude--;
	}

	delete [] temp;
}





void GGen_Data_1D::Print(){
	for(uint16 i = 0; i < length; i++) cout << i << ". :" << data[i] << "\n";
}


int main(int argc, char *argv[]){ 

	srand((unsigned)999898989); 

	uint16 octaves[5] = {10000, 5000, 2500, 1000, 100};

	/*if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		printf("Inicializace SDL se nezdaøila: %s", SDL_GetError());   
		exit(5);   
	}   
	
	SDL_SetVideoMode(255, 255, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);  */

	GGen_Data_1D* test = new GGen_Data_1D(30, 0);
	string buf;
	
	test->SetValueInRange(10, 20, 5);

	//test->Noise(-255, 255, 5, 0, octaves);

	test->Normalize(GGEN_SUBSTRACTIVE);

	//test->ScaleTo(100, false);

	//test->Flip();

	test->Print();

	cin >> buf;

	return 0;
}