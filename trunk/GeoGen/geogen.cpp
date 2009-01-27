// hide stupid "sprintf is deprecated function, use our better alternative" MSVS warnings
#define _CRT_SECURE_NO_WARNINGS

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

enum GGen_Overflow_Mode{
	GGEN_CYCLE,
	GGEN_DISCARD,
	GGEN_DISCARD_AND_FILL
};

template <class T>
T Random(T min, T max){
	double random = (double)rand() / (double) RAND_MAX;
	T output = min + (T) (random * (double)(max - min));
	return output;
}

class GGen_Data_1D{
	public:
		int16* data;
		uint16 length;

		/* Constructors/destructors */
		GGen_Data_1D(uint16 length);
		GGen_Data_1D(uint16 length, int16 value);
		GGen_Data_1D(GGen_Data_1D& victim);
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
		int16 Min();
		int16 Max();
		void Shift(int16 distance, GGen_Overflow_Mode mode);

		/* Advanced operations with array data */
		void Monochrome(int16 treshold);
		void Normalize(GGen_Normalization_Mode mode);
		void SlopeMap();
		void Gradient(uint16 from, uint16 to, int16 from_value, int16 to_value, bool fill_flat);
		void Noise(uint16 min_feature_size, uint16 max_feature_size, uint16* amplitudes);

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

/*
 * Copy constructor
 * @param victim to be cloned
 */
GGen_Data_1D::GGen_Data_1D(GGen_Data_1D& victim){
	/* Allocate the array */
	data = new int16[length];

	assert(data != NULL);
	assert(victim.data != NULL);

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
	assert(distance < length && distance != 0 && distance > -length);
	
	/* Cycle mode */
	if(mode == GGEN_CYCLE){
		/* Allocate the new array */
		int16* new_data = new int16[length];

		assert(new_data != NULL);

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

	assert(new_data != NULL);

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

void GGen_Data_1D::Noise(uint16 min_frequency, uint16 max_frequency, uint16* amplitudes){
	/* Make sure length is a power of two + 1 */
	assert(((length - 1) & (length - 2)) == 0);

	assert(amplitudes != NULL);

	//int amplitude = num_octaves + octave_skip;

	//uint16 max_frequency = (uint16) log2((double) max_feature_size);
	//uint16 min_frequency = (uint16) log2((double) min_feature_size);

	uint16 wave_length = (uint16) pow(2, (double) max_frequency);

	int16** octaves = new int16*[max_frequency - min_frequency];

	for(int frequency = min_frequency; frequency <= max_frequency; frequency++){
		

		uint16 amplitude = amplitudes[frequency];

		octaves[frequency] = new int16[length];

		for(uint16 i = 0; i < length; i++){
			octaves[frequency][i] = GGEN_INVALID_HEIGHT;
		}

		/* Fill the data */
		for(uint16 i = 0; i < length; i += wave_length){
			octaves[frequency][i] = Random((int16) -amplitude, (int16) amplitude);
		}

		/* Interpolate */
		for(uint16 i = 0; i < length; i++){
			if(octaves[frequency][i] != GGEN_INVALID_HEIGHT) continue;
			double rem = (double) (i % wave_length) / (double) wave_length;
			uint16 prev = i - i % wave_length;
			uint16 next = i - i % wave_length + wave_length;
			octaves[frequency][i] = octaves[frequency][prev]*(1-rem) + octaves[frequency][next]*rem;
			
		}

		for(uint16 i = 0; i < length; i++){
		//cout << frequency << "-" << i << "-" << octaves[frequency][i]<<"\n";
			//cout << octaves[frequency][i] << ";";
		}
		//cout << "END\n";

		wave_length /= 2;

		cout << amplitude << "-";
	}
	
	cout << "\n";

	for(uint16 i = 0; i < length; i++){
		int16 value = 0;
		for(int frequency = min_frequency; frequency <= max_frequency; frequency++){
			value += octaves[frequency][i];
		}
		data[i] = value;
	}

	//Print();

	/* Clear the height array */
	/*for(uint16 i = 0; i < length; i++){
		data[i] = GGEN_INVALID_HEIGHT;
	}

	while(amplitude >= octave_skip){
		uint16 step_size = (uint16) pow(2, (double) amplitude);

		memcpy(temp, data, length * sizeof int16);*/

		/* Skip some steps if the length is too small */
		/*if(step_size > length){
			amplitude--;
			continue;
		}

		for(uint16 i = 0; i < length; i += step_size){*/
			/* Do not interpolate the value during the first step */
			/*if(step_size != length - 1 && data[i] == GGEN_INVALID_HEIGHT){
				data[i] = (temp[i - step_size] + temp[i + step_size]) / 2;
				data[i] += Random((int16) -octaves[num_octaves - amplitude - 1], (int16) octaves[num_octaves - amplitude - 1]);
			}
			else if(step_size != length - 1){
				data[i] += Random((int16) -octaves[num_octaves - amplitude - 1], (int16) octaves[num_octaves - amplitude - 1]);
			}

			else data[i] = Random((int16) -octaves[num_octaves - amplitude - 1], (int16) octaves[num_octaves - amplitude - 1]);
			
		}*/

		//cout << "NOISE"/*<<amplitude*/ << "=" <<(signed) step_size << " - " << octaves[num_octaves - amplitude - 1] << "\n";

		/*for(uint16 i = 0; i < length; i++){
			cout << data[i]<<",";
		}*/

		//Print();

		//cout << "dsf\n";

		//amplitude--;
	//}

	//delete [] temp;
}





void GGen_Data_1D::Print(){
	for(uint16 i = 0; i < length; i++) cout << /*i << ". :" << */data[i] << "\n";
}


int main(int argc, char *argv[]){ 
/*
	IMPORTANT: The code from this below is just example of usage
*/


	srand((unsigned)5); 

	uint16 octaves[8] = {10000, 5000, 3000, 2000, 1000,750,500,250};

	/*GGen_Data_1D* a = new GGen_Data_1D(30);
	a->Gradient(0,29,-10,20,true);
	a->Monochrome(0);
	a->Print();*/

	/*GGen_Data_1D b(10);
	b.Gradient(0,9,1,10,true);
	b.Shift(-4,GGEN_DISCARD_AND_FILL);
	b.Print();*/

	/*if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		printf("Inicializace SDL se nezdaøila: %s", SDL_GetError());   
		exit(5);   
	}   
	
	SDL_SetVideoMode(255, 255, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);  */

	GGen_Data_1D* test = new GGen_Data_1D(129, 0);
	
	
	//test->SetValueInRange(10, 20, 5);

	test->Noise(0,6, octaves);

	//test->Normalize(GGEN_SUBSTRACTIVE);

	//test->ScaleTo(100, false);

	//test->Flip();
	//test->Normalize(GGEN_ADDITIVE);
	test->SlopeMap();
	test->Print();

	string buf;
	cin >> buf;

	return 0;
}