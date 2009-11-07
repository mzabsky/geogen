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

#include <iostream> // for debugging purposes

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
GGen_Data_2D::GGen_Data_2D(GGen_Size width	, GGen_Size height){
	GGen_Script_Assert(width > 1 && height > 1);

	length = width * height;
	this->width = width;
	this->height = height;

	/* Allocate the array */
	data = new GGen_Height[length];

	GGen_Script_Assert(data != NULL);

	Fill(0);
}

/** 
 * Creates a 2D data array and fills it with a constant value
 * @param width of the array
 * @param height of the array
 * @param value to be filled with
 */
GGen_Data_2D::GGen_Data_2D(GGen_Size width, GGen_Size height, GGen_Height value){
	GGen_Script_Assert(width > 1 && height > 1);
	
	length = width * height;
	this->width = width;
	this->height = height;

	/* Allocate the array */
	data = new GGen_Height[length];

	GGen_Script_Assert(data != NULL);

	Fill(value);
}

/*
 * Copy constructor
 * @param victim to be cloned
 */
GGen_Data_2D::GGen_Data_2D(GGen_Data_2D& victim){
	/* Allocate the array */
	data = new GGen_Height[victim.length];

	GGen_Script_Assert(data != NULL);
	GGen_Script_Assert(victim.data != NULL);

	/* Copy the data */
	memcpy(data, victim.data, sizeof GGen_Height * victim.length);
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
GGen_Height GGen_Data_2D::GetValue(GGen_Coord x, GGen_Coord y){
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
GGen_Height GGen_Data_2D::GetValue(GGen_Coord x, GGen_Coord y, GGen_Size scale_to_width, GGen_Size scale_to_height){
	// TODO: poresit polozky na zacatku a konci pole
	GGen_Script_Assert(y < scale_to_height && x < scale_to_width);

	/* No interpolation needed if the sizes are equal */
	if(scale_to_width == width && scale_to_height == height) return data[x + width * y];

	GGen_Height value_y_left, value_y_right;	
	
	double ratio_x = (double) (scale_to_width - 1) / (double) (width - 1);
	double ratio_y = (double) (scale_to_height - 1) / (double) (height - 1);

	/* How much does the source tile overlap over the smaller grid? */
	double remainder_x = (x / ratio_x) - floor(x / ratio_x);
	double remainder_y = (y / ratio_y) - floor(y / ratio_y);

	/* The grid anchor points */
	GGen_Coord base_x = scale_to_width > width ? (GGen_Coord) floor((double)x / ratio_x) : (GGen_Coord) floor((double)x / ratio_x + 0.5);
	GGen_Coord base_y = scale_to_height > height ? (GGen_Coord) floor((double)y / ratio_y) : (GGen_Coord) floor((double)y / ratio_y + 0.5);

	// Calculate the interpolated value for vertical axis using bilinear interpolation algorithm
	if(scale_to_height > height){
		if(remainder_y == 0){
			value_y_left = data[base_x + width * base_y];
			value_y_right = data[base_x + 1 + width * base_y];
		}

		else{
			value_y_left = (GGen_Height) ((double) data[(GGen_Coord) base_x + width * base_y] * (1 - remainder_y) + (double) data[base_x + width * (base_y + 1)] * (remainder_y));
			value_y_right = (GGen_Height) ((double) data[(GGen_Coord) base_x + 1 + width * base_y] * (1 - remainder_y) + (double) data[base_x + 1 + width * (base_y + 1)] * (remainder_y));
		}
	}
	else{
		value_y_left = value_y_right = (GGen_Height) data[(GGen_Coord) base_x + width * base_y];
	}

	/* Calculate the interpolated value for horizontal axis */
	if(scale_to_width > width){
		if(remainder_x == 0) return value_y_left;

		return (GGen_Height) ((double) value_y_left * (1 - remainder_x) + (double) value_y_right * (remainder_x));

		
	}
	else{
		return ((GGen_Height) data[(GGen_Coord) base_x + width * base_y] + value_y_left) /2;
	}
}

/** 
 * Sets value on coordinate
 * @param x coordinate
 * @param y coordinate
 * @param value to use
 */
void GGen_Data_2D::SetValue(GGen_Coord x, GGen_Coord y, GGen_Height value){
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
void GGen_Data_2D::SetValueInRect(GGen_Coord x1, GGen_Coord y1, GGen_Coord x2, GGen_Coord y2, GGen_Height value){
	GGen_Script_Assert(x2 < width && y2 < height);
	GGen_Script_Assert(x1 <= x2 && y1 <= y2);

	for(GGen_Coord y = y1; y <= y2; y++){
		for(GGen_Coord x = x1; x <= x2; x++){
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
void GGen_Data_2D::ScaleTo(GGen_Size new_width, GGen_Size new_height, bool scale_values){
	GGen_Script_Assert(new_width > 1 && new_height > 1);

	/* Pick the ratio for values as arithmetic average of horizontal and vertical ratios */
	double ratio = ((double) new_width / (double) width + (double) new_height / (double) height) / 2.0;

	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[new_width * new_height];

	GGen_Script_Assert(new_data != NULL);

	/* Fill the new array */
	for(GGen_Coord y = 0; y < new_height; y++){
		for(GGen_Coord x = 0; x < new_width; x++){
			new_data[x + y * new_width] = scale_values ? (GGen_Height) ((double) GetValue(x , y, new_width, new_height) * ratio) : GetValue(x , y, new_width, new_height);
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
void GGen_Data_2D::ScaleValuesTo(GGen_Height new_min, GGen_Height new_max)
{
	GGen_Script_Assert(new_max > new_min);

	GGen_ExtHeight min = Min();
	GGen_ExtHeight max = Max() - min;

	GGen_ExtHeight ext_new_min = new_min;
	GGen_ExtHeight ext_new_max = (GGen_ExtHeight) new_max - ext_new_min;

	if(max == 0) return;

	for(GGen_Index i = 0; i < length; i++){
		data[i] = (GGen_Height) (ext_new_min + ((GGen_ExtHeight) data[i] - min) * ext_new_max / max);
	}
}

/**
 * Change size of the array 
 * @param ratio in scale 100% = 1, 0.5 = 50%, 2.0 = 200%
 * @param scale_values - should the values be scaled too?
 */
void GGen_Data_2D::Scale(double ratio, bool scale_values){
	ScaleTo((GGen_Size) ((GGen_Size) (double) width * ratio), (GGen_Size) ((double) height * ratio), scale_values);
}

void GGen_Data_2D::ResizeCanvas(GGen_Size new_width, GGen_Size new_height, GGen_Height new_zero_x, GGen_Height new_zero_y){
	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[new_width * new_height];

	GGen_Script_Assert(new_data != NULL);

	for(GGen_Coord y = 0; y < new_height; y++){
		for(GGen_Coord x = 0; x < new_width; x++){
			if(y + new_zero_y >= 0 && y + new_zero_y < height && x + new_zero_x >= 0 && x + new_zero_x < width){
				new_data[x + y * new_width] = data [x + new_zero_x + (y + new_zero_y) * width];
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
void GGen_Data_2D::Fill(GGen_Height value){
	for(GGen_Index i = 0; i < length; i++) data[i] = value;
}

/** 
 * Adds a flat value to each value in the array
 * @param value to be used
 */
void GGen_Data_2D::Add(GGen_Height value){
	for(GGen_Index i = 0; i < length; i++) data[i] += value;
}


/** 
 * Combines the array with second array by just adding them together
 * @param addend to be combined with
 */
void GGen_Data_2D::Add(GGen_Data_2D* addend){
	/* Scale the addend as necessary */
	for(GGen_Coord y = 0; y < height; y++) 
	{
		for(GGen_Coord x = 0; x < width; x++)
		{
			data[x + y * width] += addend->GetValue(x, y , width, height);
		}
	}
}

void GGen_Data_2D::ReplaceValue(GGen_Height needle, GGen_Height replace){
	for(GGen_Coord y = 0; y < height; y++) 
	{
		for(GGen_Coord x = 0; x < width; x++)
		{
			if(data[x + y * width] == needle) data[x + y * width] = replace;
		}
	}
}

/*
 * Adds values from (unscaled) addend to the array
 * @param x offset of the addend coords
 * @param y offset of the addend coords
 * @param addend - the second array
 */
void GGen_Data_2D::AddTo(GGen_CoordOffset offset_x, GGen_CoordOffset offset_y, GGen_Data_2D* addend){
	/* Walk through the items where the array and the addend with offset intersect */
	for(GGen_Coord y = MAX(0, offset_y); y < MIN(height, offset_y + addend->height); y++){
		for(GGen_Coord x = MAX(0, offset_x); x < MIN(width, offset_x + addend->width); x++){
			data[x + y * width] += addend->data[(x - offset_x) + (y - offset_y) * addend->width];
		}
	}
}

/*
 * Combines values from the current array and the addend by adding them together.
 * The weight of data from the addend depends on values in the mask.

 */
void GGen_Data_2D::AddMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative){
	int32 min = 0;
	int32 max = 255;

	if(relative){
		min = mask->Min();
		max = mask->Max() - min;
	}
	
	if(max == 0) return;

	for(GGen_Coord y = 0; y < height; y++) 
	{
		for(GGen_Coord x = 0; x < width; x++)
		{
			data[x + y * width] += (int32) addend->GetValue(x, y, width, height) * ((int32) mask->GetValue(x, y, width, height) - min) / max;
		}
	}
}

void GGen_Data_2D::AddMasked(GGen_Height value, GGen_Data_2D* mask, bool relative){
	GGen_Height min = 0;
	GGen_Height max = 255;

	if(relative){
		min = mask->Min();
		max = mask->Max() - min;
	}

	if(max == 0) return;

	for(GGen_Coord y = 0; y < height; y++) 
	{
		for(GGen_Coord x = 0; x < width; x++)
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
	for(GGen_Index i = 0; i < length; i++) data[i] = (GGen_Coord) (factor * (double) data[i]);
}


/** 
 * Multiplies current array by the factor
 * @param factor to be combined with
 */
void GGen_Data_2D::Multiply(GGen_Data_2D* factor){
	/* Scale the factor as necessary */
	for(GGen_Coord y = 0; y < height; y++) 
	{
		for(GGen_Coord x = 0; x < width; x++)
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
	for(GGen_Index i = 0; i < length; i++) data[i] = - data[i];
}


/*
 * Returns the lowest value in the array
 */
GGen_Height GGen_Data_2D::Min(){
	GGen_Height temp = GGEN_MAX_HEIGHT;

	for(GGen_Index i = 0; i < length; i++){
		temp = temp > data[i] ? data[i] : temp;
	}

	return temp;
}

/*
 * Returns the highest value in the array
 */
GGen_Height GGen_Data_2D::Max(){
	GGen_Height temp = GGEN_MIN_HEIGHT;

	for(GGen_Index i = 0; i < length; i++){
		temp = temp < data[i] ? data[i] : temp;
	}

	return temp;
}

/*
 * Clamps all values in the array to certain range
 * @param minimum value of the range
 * @param maximum value of the range
 */
void GGen_Data_2D::Clamp(GGen_Height min, GGen_Height max){
	GGen_Script_Assert(max > min);

	for(GGen_Index i = 0; i < length; i++){
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
	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){	
			data[x + y * width] = MAX(data[x + y * width], victim->GetValue(x, y, width, height));
		}
	}
}

void GGen_Data_2D::UnionTo(GGen_CoordOffset offset_x, GGen_CoordOffset offset_y, GGen_Data_2D* victim){
	/* Walk through the items where the array and the victim with offset intersect */
	for(GGen_Coord y = MAX(0, offset_y); y < MIN(height, offset_y + victim->height); y++){
		for(GGen_Coord x = MAX(0, offset_x); x < MIN(width, offset_x + victim->width); x++){
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
	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){	
			data[x + y * width] = MIN(data[x + y * width], victim->GetValue(x, y, width, height));
		}
	}
}

void GGen_Data_2D::IntersectionTo(GGen_CoordOffset offset_x, GGen_CoordOffset offset_y, GGen_Data_2D* victim){
	/* Walk through the items where the array and the addend with offset intersect */
	for(GGen_Coord y = MAX(0, offset_y); y < MIN(height, offset_y + victim->height); y++){
		for(GGen_Coord x = MAX(0, offset_x); x < MIN(width, offset_x + victim->width); x++){
			data[x + y * width] = MIN(victim->data[(x - offset_x) + (y - offset_y) * victim->width], data[x + y * width]);
		}
	}
}

void GGen_Data_2D::Combine(GGen_Data_2D* victim, GGen_Data_2D* mask, bool relative){
	GGen_Height min = 0;
	GGen_Height max = 255;

	if(relative){
		min = mask->Min();
		max = mask->Max() - min;
	}
	
	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){	
			data[x + y * width] = ( data[x + y * width] * (mask->GetValue(x, y, width, height) - min) + victim->GetValue(x, y, width, height) * (max - mask->GetValue(x, y, width, height) + min))/ max;
		}
	}
}

void GGen_Data_2D::Abs(){
	for(GGen_Index i = 0; i < length; i++){
		if(data[i] < 0) data[i] = - data[i];
	}
}

/*
 * Projects 1D array onto a 2D array
 * @param profile to be projected
 * @param direction of projection
 */
void GGen_Data_2D::Project(GGen_Data_1D* profile, GGen_Direction direction){
	if(direction == GGEN_HORIZONTAL){
		for(GGen_Coord y = 0; y < height; y++){
			for(GGen_Coord x = 0; x < width; x++){		
				data[x + y * width] = profile->GetValue(y, height);
			}
		}
	}
	else{
		for(GGen_Coord y = 0; y < height; y++){
			for(GGen_Coord x = 0; x < width; x++){		
				data[x + y * width] = profile->GetValue(x, width);
			}
		}
	}


}

void GGen_Data_2D::Shift(GGen_Data_1D* profile, GGen_Direction direction, GGen_Overflow_Mode mode){
	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[length];

	GGen_Script_Assert(new_data != NULL);

	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){		
			if(direction == GGEN_VERTICAL){
				GGen_Height distance = profile->GetValue(x, width);

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
				GGen_Height distance = profile->GetValue(y, height);

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

void GGen_Data_2D::Gradient(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Data_1D* pattern, bool fill_outside){
	int64 target_x = to_x - from_x;
	int64 target_y = to_y - from_y;
	
	/* Width of the gradient strip */
	double max_dist = sqrt((double) (abs(to_x - from_x) * abs(to_x - from_x) + abs(to_y - from_y) * abs(to_y - from_y)));

	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){
			int64 point_x = x - from_x;
			int64 point_y = y - from_y;

			/* Get the point on the gradient vector (vector goint through both starting and target point) to which is the current point closest */
			int64 cross_x = (target_x * (target_x * point_x + target_y * point_y)) / (target_x * target_x + target_y * target_y);
			int64 cross_y = (target_y * (target_x * point_x + target_y * point_y)) / (target_x * target_x + target_y * target_y);		
		
			/* Calculate the distance from the "from" point to the intersection with gradient vector */
			int32 distance = (int32) sqrt((double) (cross_x*cross_x + cross_y*cross_y));
			
			/* Distance from  the intersection point to the target point */
			int32 reverse_distance = (int32) sqrt((double) ( ABS(target_x - cross_x) * ABS(target_x - cross_x) + ABS(target_y - cross_y) * ABS(target_y - cross_y) ));
			
			/* Apply it to the array data */
			if(distance < max_dist && reverse_distance < max_dist) {
				data[x + width * y] = pattern->GetValue(distance, (GGen_Size) max_dist);
			} 
			else if(fill_outside && reverse_distance < distance) data[x + width * y] = pattern->GetValue(pattern->length - 1);
			else if(fill_outside) data[x + width * y] = pattern->GetValue(0);
		}
	}
	
	
}

void GGen_Data_2D::Gradient(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Height from_value, GGen_Height to_value, bool fill_outside){
	/* Call the profile gradient with linear profile */
	
	GGen_Data_1D temp(2);
	temp.SetValue(0, from_value);
	temp.SetValue(1, to_value);

	Gradient(from_x, from_y, to_x, to_y, &temp, fill_outside);
}

void GGen_Data_2D::RadialGradient(GGen_Coord center_x, GGen_Coord center_y, GGen_Distance radius, GGen_Data_1D* pattern, bool fill_outside){
	GGen_Script_Assert(radius > 0 && pattern != NULL);

	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){
			GGen_Distance distance = (GGen_Distance) sqrt((double) (abs(x - center_x) * abs(x - center_x) + abs(y - center_y) * abs(y - center_y)));
		 
			if(distance < radius) data[x + width * y] = pattern->GetValue(distance, radius);
			else if (fill_outside) data[x + width * y] = pattern->GetValue(pattern->length - 1);
		}
	}
}

void GGen_Data_2D::RadialGradient(GGen_Coord center_x, GGen_Coord center_y, GGen_Coord radius, GGen_Height min, GGen_Height max, bool fill_outside){
	GGen_Script_Assert(radius > 0);
	
	GGen_ExtExtHeight rel_max = max - min;

	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){
			GGen_Distance distance = (GGen_Distance) sqrt((double) (abs(x - center_x) * abs(x - center_x) + abs(y - center_y) * abs(y - center_y)));

			if(distance < radius)  data[x + width * y] = (GGen_Height) ((GGen_ExtExtHeight) min + rel_max * (GGen_ExtExtHeight) distance / (GGen_ExtExtHeight) radius);
			else if(fill_outside) data[x + width * y] = (GGen_Height) ((GGen_ExtHeight) min + rel_max);
		}
	}
}

void GGen_Data_2D::Noise(GGen_Size min_feature_size, GGen_Size max_feature_size, GGen_Amplitudes* amplitudes){
	GGen_Script_Assert(amplitudes != NULL);

	/* Prepare empty space for the work data */ 
	GGen_Height* new_data = new GGen_Height[length];

	GGen_Script_Assert(new_data != NULL);

	Fill(0);

	/* For each octave (goind from the higher wave lengths to the shorter)... */
	for(GGen_Size wave_length = max_feature_size; wave_length >= 1; wave_length /= 2){
		uint16 frequency = GGen_log2(wave_length);
		GGen_Height amplitude = amplitudes->data[frequency];
		double pi_by_wave_length = 3.1415927 / wave_length;
		
		/* The wave length is shorter than the minimum desired wave length => done */
		if(wave_length < min_feature_size) break;

		/* Set up base noise grid values for  this round */
		for(GGen_Coord y = 0; y < height; y += wave_length){
			for(GGen_Coord x = 0; x < width; x += wave_length){
				new_data[x + y * width] = GGen_Random<int>(-amplitude, amplitude);
			}
		}		

		if(wave_length > 1)
		for(GGen_Coord y = 0; y < height; y++){
			/* Precalculate some interpolation related values that are the same for whole */
			GGen_Coord vertical_remainder = y % wave_length;
			GGen_Coord nearest_vertical = y - vertical_remainder;
			double vertical_fraction = (1 - cos(vertical_remainder * pi_by_wave_length)) * .5;
		
			uint32 vertical_offset = nearest_vertical * width;
			uint32 vertical_offset_next = (nearest_vertical + wave_length) * width;
		
			for(GGen_Coord x = 0; x < width; x++){
				/* We are on the grid ==> no need for the interpolation */
				if(vertical_remainder == 0 && x % wave_length == 0) continue;
 
				/* Nearest horizontal noise grid coordinates */
				GGen_Coord nearest_horizontal = x - x % wave_length;

				/* Fetch values of four corners so we can interpolate the correct value. If such points don't 
				exist, wrap to the opposite border and pick a point from the opposite part of the array. This is
				not an attempt to create seamlessly repeatable noise (which has no point while creating terrains).
				The source points are picked just to have some data to interpolate with (to prevent cretion of
				unpretty unnatural artifacts) */
				
				/* Upper left corner */
				GGen_Height upper_left = new_data[
					nearest_horizontal +
					vertical_offset
				];

				/* Upper right corner */
				GGen_Height upper_right;
				if(nearest_horizontal + wave_length > width - 1){
					upper_right = new_data[vertical_offset];
				}
				/* The X coord of the point overflows the right border of the map */
				else{ 
					upper_right = new_data[
						nearest_horizontal + wave_length +
						vertical_offset
					];	
				}
				
				/* Bottom left corner */
				GGen_Height bottom_left;
				if(nearest_vertical + wave_length > height - 1){
					bottom_left = new_data[nearest_horizontal];
				}
				/* The Y coord of the point overflows the bottom border of the map */
				else{
					bottom_left = new_data[
						nearest_horizontal +
						vertical_offset_next
					];	
				}

				/* Bottom right corner */
				GGen_Height bottom_right;
				/* Both coords of the point overflow the borders of the map */
				if((nearest_horizontal + wave_length > width - 1 && nearest_vertical + wave_length > height - 1) ){
					bottom_right = new_data[0];
				}
				/* The X coord of the point overflows the right border of the map */
				else if(nearest_horizontal + wave_length > width - 1){
					bottom_right = new_data[vertical_offset_next];
				}
				/* The Y coord of the point overflows the bottom border of the map */
				else if(nearest_vertical + wave_length > height - 1){
					bottom_right = new_data[nearest_horizontal + wave_length];
				}
				/* Product of the coords owerflows the length of the array */
				else if( nearest_horizontal + wave_length + vertical_offset_next > (signed) (length - 1)){
					bottom_right = new_data[0];
				}
				else{
					bottom_right = new_data[
						nearest_horizontal + wave_length +
						vertical_offset_next
					];	
				}

				/* Interpolate the value for the current tile from values of the four corners (using cosine interpolation) */
				double horizontal_fraction = (1 - cos( (x % wave_length) * pi_by_wave_length)) * .5;
				
				double interpolated_top = upper_left * (1 - horizontal_fraction) +  upper_right* horizontal_fraction;
				double interpolated_bottom = bottom_left * (1 - horizontal_fraction) + bottom_right * horizontal_fraction;

				data[x + y * width] +=(GGen_Height) (interpolated_top * ( 1 - vertical_fraction) + interpolated_bottom * vertical_fraction);
			}
		} 
		
		/* Add the current octave to previous octaves */
		for(GGen_Coord y = 0; y < height; y += wave_length){
			for(GGen_Coord x = 0; x < width; x += wave_length){
				data[x + y * width] += new_data[x + y * width];
			}
		}
	}

	delete [] new_data;
} 

void GGen_Data_2D::Noise(GGen_Size min_feature_size, GGen_Size max_feature_size){
	Noise(min_feature_size, max_feature_size, ggen_std_noise);
}

/*
 * Shifts the array values so given percentage of it is under zero (zero represents the water level).
 * @param percentage of the map to be flooded
 */
void GGen_Data_2D::Flood(double water_amount){
	GGen_Script_Assert(water_amount < 1 && water_amount > 0);

	GGen_Index target = (uint32) (water_amount * (double) length);

	GGen_Height min = Min();
	GGen_Height max = Max();

	// Use the binary search algorithm to find the correct water level
	GGen_Height middle;
	while(max - min > 1){
		middle = min + (max - min) / 2;

		GGen_Index amount = 0;
		for(GGen_Index i = 0; i < length; i++) {
			if(data[i] >= middle) amount++;
		}

		if(amount > target) min = middle;
		else max = middle;
	}

	/* Shift the heights so given portion of the array is under zero */
	Add(-middle);
}

void GGen_Data_2D::Smooth(GGen_Distance radius){
	Smooth(radius, GGEN_HORIZONTAL);
	Smooth(radius, GGEN_VERTICAL);
}

void GGen_Data_2D::Smooth(GGen_Distance radius, GGen_Direction direction){
	GGen_Script_Assert(radius > 0 && radius < width && radius < height);
	
	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[length];

	GGen_Script_Assert(new_data != NULL);

	/* Calculate size of the filter window */
	GGen_Size window_size = radius * 2 + 1;

	if(direction == GGEN_HORIZONTAL){
		for(GGen_Coord y = 0; y < height; y++){
			/* Prefill the window with value of the left edge + n leftmost values (where n is radius) */
			int32 window_value = data[width * y] * radius;

			for(GGen_Distance x = 0; x < radius; x++){
				window_value += data[x + width * y];
			}

			/* In every step shift the window one tile to the right  (= substract its leftmost cell and add
			value of rightmost + 1). i represents position of the central cell of the window. */
			for(GGen_Coord x = 0; x < width; x++){
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

		for(GGen_Coord x = 0; x < width; x++){
			/* Prefill the window with value of the left edge + n topmost values (where n is radius) */
			int32 window_value = data[x] * radius;

			for(GGen_Distance y = 0; y < radius; y++){
				window_value += data[x + y * width];
			}

			/* In every step shift the window one tile to the bottom  (= substract its topmost cell and add
			value of bottommost + 1). i represents position of the central cell of the window. */
			for(GGen_Coord y = 0; y < height; y++){
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
	
	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){
			data[x + y * width] = pattern->data[ y % pattern->width + (x % pattern->height) * pattern->height];
		}
	}
}

void GGen_Data_2D::ReturnAs(const SqPlus::sq_std_string &name){
	if(ggen_current_object->return_callback == NULL) ggen_current_object->ThrowMessage("The script returned a named map, but return handler was not defined", GGEN_WARNING);

	char* buf = GGen_ToCString(name);

	/* Call the defined return callback */
	ggen_current_object->return_callback(buf, new_data, width, height);
	
	delete [] buf;
}

void GGen_Data_2D::Monochrome(GGen_Height treshold){
	for(GGen_Index i = 0; i < length; i++){
		data[i] = data[i] > treshold ? 1 : 0;
	}	
}

void GGen_Data_2D::SelectValue(GGen_Height value){
	for(GGen_Index i = 0; i < length; i++){
		data[i] = data[i] == value ? 1 : 0;
	}	
}


void GGen_Data_2D::SlopeMap(){

	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[length];

	GGen_Script_Assert(new_data != NULL);
	GGen_Script_Assert(width > 2 && height > 2);

	for(GGen_Coord y = 1; y < height - 1; y++){
		for(GGen_Coord x = 1; x < width - 1; x++){		
			new_data[x + y * width] = MAX(abs(data[x + y * width - 1] - data[x + y * width + 1]), abs(data[x + y * width - width] - data[x + y * width + width]));
		}
	}

	new_data[0] =  new_data[width + 1];
	new_data[width - 1] = new_data[2 * width - 2];
	new_data[length - width] = new_data[length - 2 * width + 1];
	new_data[length - 1] = new_data[length - width - 2];

	// upper border
	for(GGen_Coord x = 1; x < width - 1; x++){
		new_data[x] = new_data[x + width];
	}

	// bottom border
	for(GGen_Coord x = 1; x < width - 1; x++){
		new_data[length - width + x] = new_data[length - 2 * width + x];
	}

	// left border
	for(GGen_Coord y = 1; y < height - 1; y++){
		new_data[y * width] = new_data[y * width + 1];
	}

	// right border
	for(GGen_Coord y = 1; y < height - 1; y++){
		new_data[y * width + width - 1] = new_data[y * width + width - 2];
	}

	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;	
}

void GGen_Data_2D::Scatter(bool relative){
	GGen_Height min = 0;
	GGen_Height max = 255;

	if(relative){
		min = Min();
		max = Max() - min;
	}

	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){		
			data[x + y * width] = GGen_Random(min, max) > data[x + y * width] ? 0 : 1;
		}
	}
}

void GGen_Data_2D::TransformValues(GGen_Data_1D* profile, bool relative){
	GGen_Height min = -255;
	GGen_Height max = 255;

	if(relative){
		min = Min();
		max = Max() - min;
	}
	
	/* Smoothen the profile to prevent visible color jumps in the result */
	GGen_Data_1D profile_copy(*profile);
	profile_copy.ScaleTo(max - min + 1, false);
	if(profile_copy.length > 80) profile_copy.Smooth((max - min) / 40);
	
	/* Make sure the smoothing didn't chhange the extremes */
	profile_copy.ScaleValuesTo(profile->Min(), profile->Max());
	
	/* Transform the values */
	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){		
			data[x + y * width] = profile_copy.GetValue(data[x + y * width] - min);
		}
	}
}

void GGen_Data_2D::Normalize(GGen_Direction direction){
	if(direction == GGEN_HORIZONTAL){
		for(GGen_Coord y = 0; y < height; y++){
			GGen_Height last = data[y * width];
			for(GGen_Coord x = 0; x < width; x++){
				if(data[x + y * width] > last + 1) data[x + y * width] = last + 1;
				else if(data[x + y * width] < last - 1) data[x + y * width] = last - 1;
				last = data[x + y * width];
			}
		}
	}
	else if(direction == GGEN_VERTICAL){
		for(GGen_Coord x = 0; x < width; x++){
			GGen_Height last = data[x];
			for(GGen_Coord y = 0; y < height; y++){
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




void GGen_Data_2D::Transform(double a11, double a12, double a21, double a22, bool preserve_size){
	/* The matrix must be invertible (its determinant must not be 0) */
	GGen_Script_Assert(a11 * a22 - a12 * a21 != 0);

	/* Calculate output's boundaries so we can allocate the new array */
	double new_top_right_x = (width - 1) * a11;
	double new_top_right_y = (width - 1) * a21;
	
	double new_bottom_left_x = (height - 1) * a12;
	double new_bottom_left_y = (height - 1) * a22;
	
	double new_bottom_right_x = (width - 1) * a11 + (height - 1) * a12;
	double new_bottom_right_y = (width - 1) * a21 + (height - 1) * a22;
	
	/* Find which bounding point is which (the rotations and such might change this). The zeroes
	represent the origin (upper left corner), which always stays the same. */
	int32 new_left_x = (int32) floor(MIN(MIN(0, new_top_right_x), MIN(new_bottom_left_x, new_bottom_right_x)));
	int32 new_right_x = (int32) ceil(MAX(MAX(0, new_top_right_x), MAX(new_bottom_left_x, new_bottom_right_x)));
	
	int32 new_top_y = (int32)floor(MIN(MIN(0, new_top_right_y), MIN(new_bottom_left_y, new_bottom_right_y)));
	int32 new_bottom_y = (int32) ceil(MAX(MAX(0, new_top_right_y), MAX(new_bottom_left_y, new_bottom_right_y)));
	
	uint32 new_width = new_right_x - new_left_x + 1;
	uint32 new_height = new_bottom_y - new_top_y + 1;
	
	/* Make sure the output dimensions fit into a 16 bit unsigned integer, so we don't have array overflows later */
	GGen_Script_Assert(new_width < 2 << 16 && new_height < 2 << 16);
	
	/* New origin coordinates */
	int new_origin_x = -new_left_x;
	int new_origin_y = -new_top_y;
	
	/* Invert the transformation matrix */
	double inverted_a11 = a22 / ( -(a12 * a21) + a11 * a22);
	double inverted_a12 = -(a12 / (-(a12 * a21) + a11 * a22));
	double inverted_a21 = -(a21 / (-(a12 * a21) + a11 * a22));
	double inverted_a22 = a11 / (-(a12 * a21) + a11 * a22);

	int from_x, to_x, from_y, to_y;
	uint32 new_length;
	GGen_Height* new_data;
	
	if(preserve_size){
		/* Calculate boundaries of the centered box of the original size */
		from_x = ((signed) new_width - (signed) width) / 2;
		to_x = from_x + width;

		from_y = ((signed) new_height - (signed) height) / 2;
		to_y = from_y + height;
		
		new_length = length;
		new_width = width;
		new_height = height;
		
		/* Allocate the new array */
		new_data = new GGen_Height[length];
	}
	else{ 
		/* Using the new image bounding box */
		from_x = from_y = 0;
		
		to_x = new_width;
		to_y = new_height;
		
		new_length = new_width * new_height;
		
		/* Allocate the new array */
		new_data = new GGen_Height[new_length];
	}
	
	/* Go through the new array and for every tile look back into the old array (thus we need the inverted function) what is there */
	for(int32 new_y = from_y; new_y < to_y; new_y++){
		/* The second multiplication always stays the same for whole row */
		GGen_Index y_part_1 = (GGen_Height) ((new_y - new_origin_y) * inverted_a12);
		GGen_Index y_part_2 = (GGen_Height) ((new_y - new_origin_y) * inverted_a22);
		
		/* Offset from pointer from the first cell in the array to the first cell in current row */
		int32 y_offset = (new_y - from_y) * new_width;
		
		for(int32 new_x = from_x; new_x < to_x; new_x++){
			/* Calculate the original coordinates for the current "new point" by multiplying the coordinate vector of
			the desired point by inverted transformation matrix */
			int32 x = (int32) ((new_x - new_origin_x) * inverted_a11) + y_part_1;
			int32 y = (int32) ((new_x - new_origin_x) * inverted_a21) + y_part_2;
			
			/* The original point exists => use its value */
			if(x >= 0 && y >= 0 && x < width && y < height) {			
				new_data[new_x - from_x + y_offset] = data[x + y * width];
			}
			
			/* The "original point" is outside the array => such areas are filled with black */
			else new_data[new_x - from_x + y_offset] = 0;
		}
	}
	
	/* Relink and delete the original array data */
	delete [] data;
	data = new_data;
	length = new_length;
	width = new_width;
	height = new_height;
}

void GGen_Data_2D::Rotate(int32 angle, bool preserve_size){
	angle = angle % 360;
	
	double angle_double = (double) angle * 3.14159 / 180;
	
	Transform(cos(angle_double), sin(angle_double), -sin(angle_double), cos(angle_double), preserve_size);
}

void GGen_Data_2D::Shear(int32 horizontal_shear, int32 vertical_shear, bool preserve_size){
	/* Verical and horizontal shear == 1 ==> the transformation matrix would be uninvertible */
	GGen_Script_Assert(horizontal_shear != 1 || vertical_shear != 1);
	
	Transform(1, vertical_shear, horizontal_shear, 1, preserve_size);
}
 
 
/**
  * Flips the heightmap 
  * @param direction on which the heightmap will be flipped
  */
void GGen_Data_2D::Flip(GGen_Direction direction){
	Transform(
		direction == GGEN_HORIZONTAL ? 1 : -1, 
		0,
		0,
		direction == GGEN_VERTICAL ? 1 : -1,
		false
	);
} 