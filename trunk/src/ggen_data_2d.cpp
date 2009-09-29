/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ggen.h"
#include "ggen_squirrel.h"
#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"

extern GGen_Amplitudes* ggen_std_noise;
extern GGen* ggen_current_object;
/** 
 * Creates a 2D data array and fills it with zeros
 * @param length of the array
 */
GGen_Data_2D::GGen_Data_2D(uint16 width	, uint16 height){
	GGen_Script_Assert(width > 1 && height > 1);

	length = width * height;
	this->width = width;
	this->height = height;

	/* Allocate the array */
	data = new int16[length];

	GGen_Script_Assert(data != NULL);

	Fill(0);
}

/** 
 * Creates a 1D data array and fills it with a constant value
 * @param width of the array
 * @param height of the array
 * @param value to be filled with
 */
GGen_Data_2D::GGen_Data_2D(uint16 width, uint16 height, int16 value){
	GGen_Script_Assert(width > 1 && height > 1);
	
	length = width * height;
	this->width = width;
	this->height = height;

	/* Allocate the array */
	data = new int16[length];

	GGen_Script_Assert(data != NULL);

	Fill(value);
}

/*
 * Copy constructor
 * @param victim to be cloned
 */
GGen_Data_2D::GGen_Data_2D(GGen_Data_2D& victim){
	/* Allocate the array */
	data = new int16[victim.length];

	GGen_Script_Assert(data != NULL);
	GGen_Script_Assert(victim.data != NULL);

	/* Copy the data */
	memcpy(data, victim.data, sizeof int16 * victim.length);
	length = victim.length;
	width = victim.width;
	height = victim.height;
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
	GGen_Script_Assert(x < width && y < height);
	
	return data[x + width * y];
}

/** 
 * Reads and returns one value from the array as if its size was scale_to_x * scale_to_y
 * @param x coordinate of the value
 * @param y coordinate of the value
 * @param target width
 * @param target height
 */
int16 GGen_Data_2D::GetValue(uint16 x, uint16 y, uint16 scale_to_width, uint16 scale_to_height){
	// TODO: poresit polozky na zacatku a konci pole
	GGen_Script_Assert(y < scale_to_height && x < scale_to_width);

	/* No interpolation needed if the sizes are equal */
	if(scale_to_width == width && scale_to_height == height) return data[x + width * y];

	int16 value_y_left, value_y_right;	
	
	double ratio_x = (double) (scale_to_width - 1) / (double) (width - 1);
	double ratio_y = (double) (scale_to_height - 1) / (double) (height - 1);

	/* How much does the source tile overlap over the smaller grid? */
	double remainder_x = (x / ratio_x) - floor(x / ratio_x);
	double remainder_y = (y / ratio_y) - floor(y / ratio_y);

	/* The grid anchor points */
	uint16 base_x = scale_to_width > width ? (uint16) floor((double)x / ratio_x) : (uint16) floor((double)x / ratio_x + 0.5);
	uint16 base_y = scale_to_height > height ? (uint16) floor((double)y / ratio_y) : (uint16) floor((double)y / ratio_y + 0.5);

	// Calculate the interpolated value for vertical axis using bilinear interpolation algorithm
	if(scale_to_height > height){
		if(remainder_y == 0){
			value_y_left = data[base_x + width * base_y];
			value_y_right = data[base_x + 1 + width * base_y];
		}

		else{
			value_y_left = (int16) ((double) data[(uint16) base_x + width * base_y] * (1 - remainder_y) + (double) data[base_x + width * (base_y + 1)] * (remainder_y));
			value_y_right = (int16) ((double) data[(uint16) base_x + 1 + width * base_y] * (1 - remainder_y) + (double) data[base_x + 1 + width * (base_y + 1)] * (remainder_y));
		}
	}
	else{
		value_y_left = value_y_right = (int16) data[(uint16) base_x + width * base_y];
	}

	/* Calculate the interpolated value for horizontal axis */
	if(scale_to_width > width){
		if(remainder_x == 0) return value_y_left;

		return (int16) ((double) value_y_left * (1 - remainder_x) + (double) value_y_right * (remainder_x));

		
	}
	else{
		return ((int16) data[(uint16) base_x + width * base_y] + value_y_left) /2;
	}
}

/** 
 * Sets value on coordinate
 * @param x coordinate
 * @param y coordinate
 * @param value to use
 */
void GGen_Data_2D::SetValue(uint16 x, uint16 y, int16 value){
	GGen_Script_Assert(x < width && y < height);
	
	data[x + width * y] = value;
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
	GGen_Script_Assert(x2 < width && y2 < height);
	GGen_Script_Assert(x1 <= x2 && y1 <= y2);

	for(uint16 y = y1; y <= y2; y++){
		for(uint16 x = x1; x <= x2; x++){
			data[x + width * y] = value;
		}
	}

}

/**
 * Change size of the array 
 * @param width of the new array
 * @param height of the new array
 * @param scale_values - should the values be scaled too?
 */
void GGen_Data_2D::ScaleTo(uint16 new_width, uint16 new_height, bool scale_values){
	GGen_Script_Assert(new_width > 1 && new_height > 1);

	/* Pick the ratio for values as arithmetic average of horizontal and vertical ratios */
	double ratio = ((double) new_width / (double) width + (double) new_height / (double) height) / 2.0;

	/* Allocate the new array */
	int16* new_data = new int16[new_width * new_height];

	GGen_Script_Assert(new_data != NULL);

	/* Fill the new array */
	for(uint16 y = 0; y < new_height; y++){
		for(uint16 x = 0; x < new_width; x++){
			new_data[x + y * new_width] = scale_values ? (int16) ((double) GetValue(x , y, new_width, new_height) * ratio) : GetValue(x , y, new_width, new_height);
		}
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	width = new_width;
	height = new_height;
	length = new_width * new_height;
}

/**
 * Scales the values so they all fit to the passed range
 * @param new minimum value
 * @param new maximum value
 */
void GGen_Data_2D::ScaleValuesTo(int16 new_min, int16 new_max)
{
	GGen_Script_Assert(new_max > new_min);

	int16 min = Min();
	int16 max = Max() - min;

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
	ScaleTo((uint16) ((uint16) (double) width * ratio), (uint16) ((double) height * ratio), scale_values);
}

void GGen_Data_2D::ResizeCanvas(int16 new_width, int16 new_height, int16 new_zero_x, int16 new_zero_y){
	/* Allocate the new array */
	int16* new_data = new int16[new_width * new_height];

	GGen_Script_Assert(new_data != NULL);

	for(uint16 y = 0; y < new_height; y++){
		for(uint16 x = 0; x < new_width; x++){
			if(y + new_zero_y >= 0 && y + new_zero_y < height && x + new_zero_x >= 0 && x + new_zero_x){
				new_data[x + y * new_width] = data[x + new_zero_x + (y + new_zero_y) * width];
			}
			else new_data[x + y * new_width] = 0;
		}
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	length = new_width * new_height;
	width = new_width;
	height = new_height;
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
	for(uint16 y = 0; y < height; y++) 
	{
		for(uint16 x = 0; x < width; x++)
		{
			data[x + y * width] += addend->GetValue(x, y , width, height);
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
	for(uint16 y = MAX(0, offset_y); y < MIN(height, offset_y + addend->height); y++){
		for(uint16 x = MAX(0, offset_x); x < MIN(width, offset_x + addend->width); x++){
			data[x + y * width] += addend->data[(x - offset_x) + (y - offset_y) * addend->width];
		}
	}
}

/*
 * Combines values from the current array and the addend by adding them together.
 * The weight of data from the addend depends on values in the mask.

 */
void GGen_Data_2D::AddMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative){
	int16 min = 0;
	int16 max = 255;

	if(relative){
		min = Min();
		max = Max() - min;
	}

	for(uint16 y = 0; y < height; y++) 
	{
		for(uint16 x = 0; x < width; x++)
		{
			data[x + y * width] += addend->GetValue(x, y, width, height) * (mask->GetValue(x, y, width, height) - min) / max;
		}
	}
}

void GGen_Data_2D::AddMasked(int16 value, GGen_Data_2D* mask, bool relative){
	int16 min = 0;
	int16 max = 255;

	if(relative){
		min = Min();
		max = Max() - min;
	}

	for(uint16 y = 0; y < height; y++) 
	{
		for(uint16 x = 0; x < width; x++)
		{
			data[x + y * width] += value * (mask->GetValue(x, y, width, height) - min) / max;
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
	for(uint16 y = 0; y < height; y++) 
	{
		for(uint16 x = 0; x < width; x++)
		{
			data[x + y * width] *= factor->GetValue(x, y , width, height);
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
	
	if(angle == GGEN_0) return;

	// fix width and height
	uint16 new_width, new_height;
	if(angle == GGEN_180){
		new_width = width;
		new_height = height;
	}
	else{
		new_width = height;
		new_height = width;		
	}
	
	/* Allocate the new array */
	int16* new_data = new int16[new_width * new_height];

	GGen_Script_Assert(new_data != NULL);	

	/* Fill the new array with rotated values */
	for(uint16 y = 0; y < new_height; y++) 
	{
		for(uint16 x = 0; x < new_width; x++)
		{
			switch (angle){
				case GGEN_90:
					new_data[x + y * new_width] = data[y + x * width];
					break;
				case GGEN_180:
					new_data[(new_width - x - 1) + y * new_width] = data[x + y * width];
					break;
				case GGEN_270:
					new_data[x + (new_height - y - 1) * new_width] = data[y + x * width];
					break;
			}
		}
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	width = new_width;
	height = new_height;
}

/**
  * Flips the heightmap 
  * @param direction on which the heightmap will be flipped
  */
void GGen_Data_2D::Flip(GGen_Direction direction){
	int16 temp;

	if(direction == GGEN_HORIZONTAL){	
		for(uint16 y = 0; y < height; y++) 
		{
			for(uint16 x = 0; x < width / 2; x++)
			{
				temp = data[x + y * width];
				data[x + y * width] = data[(width - 1 - x) + y * width];
				data[(width - 1 - y) + x * width] = temp;
			}
		}
		int i = 1+1;
	}
	else{
		for(uint16 y = 0; y < height / 2; y++) 
		{
			for(uint16 x = 0; x < width; x++)
			{
				temp = data[x + y * width];
				data[x + y * width] = data[x + (height - 1 - y) * width];
				data[x + (height - 1 - y) * width] = temp;
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
	GGen_Script_Assert(max > min);

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
void GGen_Data_2D::Union(GGen_Data_2D* victim){
	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){	
			data[x + y * width] = MAX(data[x + y * width], victim->GetValue(x, y, width, height));
		}
	}
}

void GGen_Data_2D::UnionTo(int16 offset_x, int16 offset_y, GGen_Data_2D* victim){
	/* Walk through the items where the array and the victim with offset intersect */
	for(uint16 y = MAX(0, offset_y); y < MIN(height, offset_y + victim->height); y++){
		for(uint16 x = MAX(0, offset_x); x < MIN(width, offset_x + victim->width); x++){
			data[x + y * width] = MAX(victim->data[(x - offset_x) + (y - offset_y) * victim->width], data[x + y * width]);
		}
	}
}

/*
 * Unifies the data with data from the other array. The other array is scaled to fit the object.
 * Negative data are treated the same as positive - the higher value remains.
 * @param the victim
 */
void GGen_Data_2D::Intersection(GGen_Data_2D* victim){
	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){	
			data[x + y * width] = MIN(data[x + y * width], victim->GetValue(x, y, width, height));
		}
	}
}

void GGen_Data_2D::IntersectionTo(int16 offset_x, int16 offset_y, GGen_Data_2D* victim){
	/* Walk through the items where the array and the addend with offset intersect */
	for(uint16 y = MAX(0, offset_y); y < MIN(height, offset_y + victim->height); y++){
		for(uint16 x = MAX(0, offset_x); x < MIN(width, offset_x + victim->width); x++){
			data[x + y * width] = MIN(victim->data[(x - offset_x) + (y - offset_y) * victim->width], data[x + y * width]);
		}
	}
}

void GGen_Data_2D::Combine(GGen_Data_2D* victim, GGen_Data_2D* mask, bool relative){
	int16 min = 0;
	int16 max = 255;

	if(relative){
		min = Min();
		max = Max() - min;
	}
	
	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){	
			data[x + y * width] = ( data[x + y * width] * (mask->GetValue(x, y, width, height) - min) + victim->GetValue(x, y, width, height) * (max - mask->GetValue(x, y, width, height) + min))/ max;
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
		for(uint16 y = 0; y < height; y++){
			for(uint16 x = 0; x < width; x++){		
				data[x + y * width] = profile->GetValue(y, height);
			}
		}
	}
	else{
		for(uint16 y = 0; y < height; y++){
			for(uint16 x = 0; x < width; x++){		
				data[x + y * width] = profile->GetValue(x, width);
			}
		}
	}


}

void GGen_Data_2D::Shift(GGen_Data_1D* profile, GGen_Direction direction, GGen_Overflow_Mode mode){
	/* Allocate the new array */
	int16* new_data = new int16[length];

	GGen_Script_Assert(new_data != NULL);

	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){		
			if(direction == GGEN_VERTICAL){
				int16 distance = profile->GetValue(x, width);

				/* Some values can be just plainly shifted */
				if((distance >= 0 && y < height - distance) || (distance <= 0 && (signed) y >= -distance)){
					new_data[x + (y + distance) * width] = data[x + y * width];
				}
				/* Some must go through the upper "border" */
				else if(distance >= 0){
					if(mode == GGEN_CYCLE) new_data[x + (y - height + distance) * width] = data[x + y * width];
					else if(mode == GGEN_DISCARD_AND_FILL) new_data[x + (y - height + distance) * width] = data[x];
				}
				/* And some must go through the bottom "border" */
				else{
					if(mode == GGEN_CYCLE) new_data[x + (y + height + distance) * width] = data[x + y * width];
					else if(mode == GGEN_DISCARD_AND_FILL) new_data[x + (y + height + distance) * width] = data[x + (height - 1) * width];
				}					
			}
			else{ // GGEN_HORIZONTAL
				int16 distance = profile->GetValue(y, height);

				/* Some values can be just plainly shifted */
				if((distance >= 0 && x < width - distance) || (distance <= 0 && (signed) x >= -distance)){
					new_data[x + distance + y * width] = data[x + y * width];
				}
				/* Some must go through the right "border" */
				else if(distance >= 0){
					if(mode == GGEN_CYCLE) new_data[x - width + distance + y * width] = data[x + y * width];
					else if(mode == GGEN_DISCARD_AND_FILL) new_data[x - width + distance + y * width] = data[y * width];
				}
				/* And some must go through the left "border" */
				else{
					if(mode == GGEN_CYCLE) new_data[x + width + distance + y * width] = data[x + y * width];
					else if(mode == GGEN_DISCARD_AND_FILL) new_data[x + width + distance + y * width] = data[width - 1 + y * width];
				}					
			}
		}
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
}

void GGen_Data_2D::Gradient(uint16 from_x, uint16 from_y, uint16 to_x, uint16 to_y, GGen_Data_1D* pattern, bool fill_outside){
	/* Relative target point coordinates respective to the starting point */
	int32 target_x = to_x - from_x; 
	int32 target_y = to_y - from_y;

	/* Width of the gradient strip */
	double max_dist = sqrt((double) (abs(to_x - from_x) * abs(to_x - from_x) + abs(to_y - from_y) * abs(to_y - from_y)));

	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){
			/* Relative current point coordinates respective to the starting point */
			int32 point_x = x - from_x;
			int32 point_y = y - from_y;

			/* Get the point on the gradient vector to which is the current point closest */
			int32 cross_x = (target_x * (target_x * point_x + target_y * point_y)) / (target_x * target_x + target_y * target_y);
			int32 cross_y = (target_y * (target_x * point_x + target_y * point_y)) / (target_x * target_x + target_y * target_y);

			/* Calculate the distance from the "from" pont */
			int32 distance = (int32) sqrt((double) (cross_x * cross_x + cross_y * cross_y));
	
			// TODO: fill_outside pred tu silenou podminku
			/* Fill/skip the outside areas */
			if ((cross_x < 0 == (signed) to_x-(signed) from_x > 0) || (cross_y < 0 == (signed) to_y - (signed) from_y > 0) || distance >= floor(max_dist)){
				if(fill_outside){
					data[x + width * y] = ((cross_x < 0 == (signed) to_x - (signed) from_x >= 0) || (cross_y < 0 == (signed) to_y - (signed) from_y > 0))  ? pattern->GetValue(0) : pattern->GetValue(pattern->length - 1);
				}
				continue;
			}

			/* Apply it to the array data */
			data[x + width * y] = pattern->GetValue((uint16) distance, (uint16) max_dist);
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
	GGen_Script_Assert(radius > 0 && pattern != NULL);

	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){
			uint16 distance = (uint16) sqrt((double) (abs(x - center_x) * abs(x - center_x) + abs(y - center_y) * abs(y - center_y)));
		 
			if(distance < radius) data[x + width * y] = pattern->GetValue(distance, radius);
			else if (fill_outside) data[x + width * y] = pattern->GetValue(pattern->length - 1);
		}
	}
}

void GGen_Data_2D::RadialGradient(uint16 center_x, uint16 center_y, uint16 radius, int16 min, int16 max, bool fill_outside){
	GGen_Script_Assert(radius > 0);
	
	max = max - min;

	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){
			uint16 distance = (uint16) sqrt((double) (abs(x - center_x) * abs(x - center_x) + abs(y - center_y) * abs(y - center_y)));

			if(distance < radius)  data[x + width * y] = min + max * distance / radius;
			else if(fill_outside) data[x + width * y] = min + max;
		}
	}
}

void GGen_Data_2D::Noise(uint16 min_feature_size, uint16 max_feature_size, GGen_Amplitudes* amplitudes){

	GGen_Script_Assert(amplitudes != NULL);

	uint8 frequency = GGen_log2(max_feature_size);
	uint16 amplitude = amplitudes->data[frequency];

	int16* new_data = new int16[length];

	GGen_Script_Assert(new_data != NULL);

	Fill(0);

	for(uint16 wave_length = max_feature_size; wave_length >= 1; wave_length /= 2){
		frequency = GGen_log2(wave_length);
		amplitude = amplitudes->data[frequency];

		if(wave_length < min_feature_size) break;

		for(uint16 y = 0; y < height; y += wave_length){
			for(uint16 x = 0; x < width; x += wave_length){
				new_data[x + y * width] = GGen_Random<int>(-amplitude, amplitude);
			}
		}

		

		if(wave_length > 1)
		for(uint16 y = 0; y < height; y++){
			for(uint16 x = 0; x < width; x++){
				if(y % wave_length == 0 && x % wave_length == 0) continue;
 
				uint16 nearest_horizontal = x - x % wave_length;
				uint16 nearest_vertical = y - y % wave_length;

				//double ft = (j % wave_length) * 3.1415927
				double horizontal = (1 - cos( (x % wave_length) * 3.1415927 / wave_length)) * .5;
				double vertical = (1 - cos( (y % wave_length) * 3.1415927 / wave_length)) * .5;


				int16 a = new_data[
					nearest_horizontal +
					(nearest_vertical) * width
				];

				int16 b;
				if(nearest_horizontal + wave_length > width - 1){
					//b = a;
					b = new_data[(nearest_vertical) * width];
				}
				else 
				b = new_data[
					nearest_horizontal + wave_length +
					(nearest_vertical) * width
				];	
	
				int16 c;
				if(nearest_vertical + wave_length > height - 1){
					c = new_data[nearest_horizontal];
				}
				else 
				c = new_data[
					nearest_horizontal +
					(nearest_vertical + wave_length) * width
				];	

				int16 d;
				if((nearest_horizontal + wave_length > width - 1 && nearest_vertical + wave_length > height - 1) ){
					d = new_data[0];
				}
				else if(nearest_horizontal + wave_length > width - 1){
					d = new_data[(nearest_vertical + wave_length) * width];
				}
				else if(nearest_vertical + wave_length > height - 1){
					d = new_data[nearest_horizontal + wave_length];
				}
				else if( nearest_horizontal + wave_length + (nearest_vertical + wave_length) * width > (signed) (length - 1)){
					d = new_data[0];
				}
				else 
				d = new_data[
					nearest_horizontal + wave_length +
					(nearest_vertical + wave_length) * width
				];	

				// a*(1-f) + b*f

				double horizontal_value = a*(1-horizontal) + b*horizontal;
				double horizontal_value2 = c*(1-horizontal) + d*horizontal;
				//double vertical_value = 

				data[x + y * width] += (int16)( horizontal_value * ( 1 - vertical) + horizontal_value2 * vertical);

				//int sdf = data[j + i * x];
			}
		} 

		for(uint16 y = 0; y < height; y += wave_length){
			for(uint16 x = 0; x < width; x += wave_length){
				data[x + y * width] += new_data[x + y * width];
			}
		}

	}

	delete [] new_data;

	/* First round - generate base values */
	//for(int i = 0; i < y; i += max_feature_size){
	//	for(int j = 0; j < x; j += max_feature_size){
	//		data[j + i * x] = Random((int16) -amplitude, (int16) amplitude);
	//	}		
	//}

	//
	//for(int wave_length = max_feature_size / 2; wave_length >= 1; wave_length /= 2){
	//	frequency = log2(wave_length);
	//	amplitude = amplitudes->data[frequency];

	//	for(uint16 i = 0; i < y; i += wave_length * 2){
	//		for(uint16 j = 0; j < x; j += wave_length * 2){
	//			if(j < x - wave_length) data[j + wave_length + i * x] = ( 
	//				data[j + i * x] +
	//				(j < x - wave_length * 2 ? data[j + 2 * wave_length + i * x] : 0)
	//				) / 2 + (wave_length >= min_feature_size ? Random((int16) -amplitude, (int16) amplitude) : 0);
	//			if(i < y - wave_length) data[j + (i + wave_length) * x] = ( 
	//				data[j + i * x] +
	//				(i < y - wave_length * 2 ? data[j + (i + 2 * wave_length) * x] : 0)
	//				) / 2 + (wave_length >= min_feature_size ? Random((int16) -amplitude, (int16) amplitude) : 0);
	//			if(j < x - wave_length && i < y - wave_length) data[j + wave_length + (i + wave_length) * x] = ( 
	//				data[j + i * x] +
	//				(i < y - wave_length * 2 ? data[j + (i + 2 * wave_length) * x] : 0) +
	//				(j < x - wave_length * 2 ? data[j + 2 * wave_length + i * x] : 0) +
	//				((j < x - wave_length * 2 && i < y - wave_length * 2) ? data[j + 2 * wave_length + (i + 2 * wave_length) * x] : 0)
	//				) / 4 + (wave_length >= min_feature_size ? Random((int16) -amplitude, (int16) amplitude) : 0);
	//			if(wave_length >= min_feature_size) data[j + i * x] += Random((int16) -amplitude, (int16) amplitude);
	//		}
	//	}
	//}

} 

void GGen_Data_2D::Noise(uint16 min_feature_size, uint16 max_feature_size){
	Noise(min_feature_size, max_feature_size, ggen_std_noise);
}

/*
 * Shifts the array values so given percentage of it is under zero (zero represents the water level).
 * @param percentage of the map to be flooded
 */
void GGen_Data_2D::Flood(double water_amount){
	GGen_Script_Assert(water_amount < 1 && water_amount > 0);

	uint32 target = (uint32) (water_amount * (double) length);

	int16 min = Min();
	int16 max = Max();

	// Use the binary search algorithm to find the correct water level
	int16 middle;
	while(max - min > 1){
		middle = min + (max - min) / 2;

		uint32 amount = 0;
		for(uint32 i = 0; i < length; i++) {
			if(data[i] >= middle) amount++;
		}

		if(amount > target) min = middle;
		else max = middle;
	}

	/* Shift the heights so given portion of the array is under zero */
	Add(-middle);
}

void GGen_Data_2D::Smooth(uint8 radius){
	Smooth(radius, GGEN_HORIZONTAL);
	Smooth(radius, GGEN_VERTICAL);
}

void GGen_Data_2D::Smooth(uint8 radius, GGen_Direction direction){
	GGen_Script_Assert(radius > 0 && radius < width && radius < height);
	
	/* Allocate the new array */
	int16* new_data = new int16[length];

	GGen_Script_Assert(new_data != NULL);

	/* Calculate size of the filter window */
	uint16 window_size = radius * 2 + 1;

	if(direction == GGEN_HORIZONTAL){
		for(uint16 y = 0; y < height; y++){
			/* Prefill the window with value of the left edge + n leftmost values (where n is radius) */
			int32 window_value = data[width * y] * radius;

			for(uint8 x = 0; x < radius; x++){
				window_value += data[x + width * y];
			}

			/* In every step shift the window one tile to the right  (= substract its leftmost cell and add
			value of rightmost + 1). i represents position of the central cell of the window. */
			for(uint16 x = 0; x < width; x++){
				/* If the window is approaching right border, use the rightmost value as fill. */
				if((signed) x - (signed)radius < 0){
					window_value += data[x + radius + width * y] - data[width * y];
				}
				else if(x + radius < width){
					window_value += data[x + radius + width * y] - data[x - radius + width * y];
				}
				else{
					window_value += data[width - 1 + width * y] - data[x - radius + width * y];
				}

				/* Set the value of current tile to arithmetic average of window tiles. */
				new_data[x + width * y] = window_value / window_size;
			}
		}
	}
	else{ // vertical

		for(uint16 x = 0; x < width; x++){
			/* Prefill the window with value of the left edge + n topmost values (where n is radius) */
			int32 window_value = data[x] * radius;

			for(uint8 y = 0; y < radius; y++){
				window_value += data[x + y * width];
			}

			/* In every step shift the window one tile to the bottom  (= substract its topmost cell and add
			value of bottommost + 1). i represents position of the central cell of the window. */
			for(uint16 y = 0; y < height; y++){
				/* If the window is approaching right border, use the rightmost value as fill. */
				if((signed) y - (signed) radius < 0){
					window_value += data[x + (y + radius) * width] - data[x];
				}
				else if(y + radius < height){
					window_value += data[x + (y + radius) * width] - data[x + (y - radius) * width];
				}
				else{
					window_value += data[x + (height - 1) * width] - data[x + (y - radius) * width];
				}

				/* Set the value of current tile to arithmetic average of window tiles. */
				new_data[x + width * y] = window_value / window_size;
			}
		}
	}
	

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;	

	return;	
}

void GGen_Data_2D::Pattern(GGen_Data_2D* pattern){
	GGen_Script_Assert(pattern != NULL);
	
	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){
			data[x + y * width] = pattern->data[ y % pattern->width + (x % pattern->height) * pattern->height];
		}
	}
}

void GGen_Data_2D::ReturnAs(const SqPlus::sq_std_string &name){
	if(ggen_current_object->return_callback == NULL) ggen_current_object->ThrowMessage("The script returned a named map, but return handler was not defined", GGEN_WARNING);
	
	/* Allocate the new array */
	int16* new_data = new int16[length];

	GGen_Script_Assert(new_data != NULL);

	memcpy(new_data, data, sizeof(int16) * length);

	char* buf = GGen_ToCString(name);

	ggen_current_object->return_callback(buf, new_data, width, height);
}

void GGen_Data_2D::Monochrome(int16 treshold){
	for(uint32 i = 0; i < length; i++){
		data[i] = data[i] > treshold ? 1 : 0;
	}	
}

void GGen_Data_2D::SlopeMap(){

	/* Allocate the new array */
	int16* new_data = new int16[length];

	GGen_Script_Assert(new_data != NULL);
	GGen_Script_Assert(width > 2 && height > 2);

	for(uint16 y = 1; y < height - 1; y++){
		for(uint16 x = 1; x < width - 1; x++){		
			new_data[x + y * width] = MAX(abs(data[x + y * width - 1] - data[x + y * width + 1]), abs(data[x + y * width - width] - data[x + y * width + width]));
		}
	}

	new_data[0] =  new_data[width + 1];
	new_data[width - 1] = new_data[2 * width - 2];
	new_data[length - width] = new_data[length - 2 * width + 1];
	new_data[length - 1] = new_data[length - width - 2];

	// upper border
	for(uint16 x = 1; x < width - 1; x++){
		new_data[x] = new_data[x + width];
	}

	// bottom border
	for(uint16 x = 1; x < width - 1; x++){
		new_data[length - width + x] = new_data[length - 2 * width + x];
	}

	// left border
	for(uint16 y = 1; y < height - 1; y++){
		new_data[y * width] = new_data[y * width + 1];
	}

	// right border
	for(uint16 y = 1; y < height - 1; y++){
		new_data[y * width + width - 1] = new_data[y * width + width - 2];
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;	
}

void GGen_Data_2D::Scatter(bool relative){
	int16 min = 0;
	int16 max = 255;

	if(relative){
		min = Min();
		max = Max() - min;
	}

	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){		
			data[x + y * width] = GGen_Random(min, max) > data[x + y * width] ? 0 : 1;
		}
	}
}

void GGen_Data_2D::TransformValues(GGen_Data_1D* profile){
	bool relative = true; 
	
	int16 min = 0;
	int16 max = 255;

	if(relative){
		min = Min();
		max = Max() - min;
	}

	for(uint16 y = 0; y < height; y++){
		for(uint16 x = 0; x < width; x++){		
			data[x + y * width] = profile->GetValue(data[x + y * width] - min,max - min);
		}
	}
}

void GGen_Data_2D::Normalize(GGen_Direction direction){
	if(direction == GGEN_HORIZONTAL){
		for(uint16 y = 0; y < height; y++){
			uint16 last = data[y * width];
			for(uint16 x = 0; x < width; x++){
				if(data[x + y * width] > last + 1) data[x + y * width] = last + 1;
				else if(data[x + y * width] < last - 1) data[x + y * width] = last - 1;
				last = data[x + y * width];
			}
		}
	}
	else if(direction == GGEN_VERTICAL){
		for(uint16 x = 0; x < width; x++){
			uint16 last = data[x];
			for(uint16 y = 0; y < height; y++){
				if(data[x + y * width] > last + 1) data[x + y * width] = last + 1;
				else if(data[x + y * width] < last - 1) data[x + y * width] = last - 1;
				last = data[x + y * width];
			}
		}
	}
}

void GGen_Data_2D::Normalize(){
	Normalize(GGEN_HORIZONTAL);
	Normalize(GGEN_VERTICAL);
}
