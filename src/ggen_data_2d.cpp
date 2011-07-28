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
#include <list>
#include <queue>
#include <bitset>
#include <cstring>
#include <cmath>
#include <stack>

#include "ggen.h"
#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"
#include "ggen_path.h"
#include "ggen_erosionsimulator.h"
#include <assert.h>

uint16 GGen_Data_2D::num_instances = 0;
set<GGen_Data_2D*> GGen_Data_2D::instances;

GGen_Data_2D::GGen_Data_2D(GGen_Size width, GGen_Size height, GGen_Height value)
{
	GGen_Script_Assert(GGen::GetInstance()->GetStatus() != GGEN_LOADING_MAP_INFO);

	GGen_Script_Assert(width > 1 && height > 1);
	GGen_Script_Assert(width < GGen::GetMaxMapSize());
	GGen_Script_Assert(height < GGen::GetMaxMapSize());

	GGen_Script_Assert(GGen_Data_2D::num_instances < GGen::GetMaxMapCount());
	GGen_Data_2D::num_instances++;

	GGen_Data_2D::instances.insert(this);

	this->length = width * height;
	this->width = width;
	this->height = height;

	/* Allocate the array */
	this->data = new GGen_Height[this->length];

	GGen_Script_Assert(this->data != NULL);

	this->Fill(value);
}

GGen_Data_2D* GGen_Data_2D::Clone()
{
	GGen_Data_2D* victim = new GGen_Data_2D(this->width, this->height, 0);
	
	/* Copy the data */
	memcpy(victim->data, this->data, sizeof(GGen_Height) * this->length);

	return victim;
}

GGen_Data_2D::~GGen_Data_2D()
{
	assert(GGen_Data_2D::instances.find(this) != GGen_Data_2D::instances.end());

	GGen_Data_2D::num_instances--;
	GGen_Data_2D::instances.erase(this);
	delete [] this->data;
}

GGen_Size GGen_Data_2D::GetWidth()
{
	return this->width;
}

GGen_Size GGen_Data_2D::GetHeight()
{
	return this->height;
}

GGen_Index GGen_Data_2D::GetLength()
{
	return this->length;
}

GGen_Distance GGen_Data_2D::GetMaxDistance(){
	return (GGen_Distance) sqrt((double) this->width * (double) this->height);
}

GGen_Height GGen_Data_2D::GetValue(GGen_Coord x, GGen_Coord y)
{
	GGen_Script_Assert(x < this->width && y < this->height);
	
	return this->data[x + this->width * y];
}

GGen_Height GGen_Data_2D::GetValueInterpolated(GGen_Coord x, GGen_Coord y, GGen_Size scale_to_width, GGen_Size scale_to_height)
{
	GGen_Script_Assert(y < scale_to_height)
	GGen_Script_Assert(x < scale_to_width);
	GGen_Script_Assert(scale_to_width >= GGEN_MIN_MAP_SIZE);
	GGen_Script_Assert(scale_to_width <= GGen::GetMaxMapSize());
	GGen_Script_Assert(scale_to_height >= GGEN_MIN_MAP_SIZE);
	GGen_Script_Assert(scale_to_height <= GGen::GetMaxMapSize());

	/* No interpolation needed if the sizes are equal */
	if (scale_to_width == width && scale_to_height == height) {
		return data[x + width * y];
	}

	GGen_Height value_y_left, value_y_right;	
	
	double ratio_x = (double) (scale_to_width - 1) / (double) (this->width - 1);
	double ratio_y = (double) (scale_to_height - 1) / (double) (this->height - 1);

	/* How much does the source tile overlap over the smaller grid? */
	double remainder_x = (x / ratio_x) - floor(x / ratio_x);
	double remainder_y = (y / ratio_y) - floor(y / ratio_y);

	/* The grid anchor points */
	GGen_Coord base_x = scale_to_width > this->width ? (GGen_Coord) floor((double)x / ratio_x) : (GGen_Coord) floor((double)x / ratio_x + 0.5);
	GGen_Coord base_y = scale_to_height > this->height ? (GGen_Coord) floor((double)y / ratio_y) : (GGen_Coord) floor((double)y / ratio_y + 0.5);

	/* Calculate the interpolated value for vertical axis using bilinear interpolation algorithm */
	if (scale_to_height > this->height) {
		if (remainder_y == 0) {
			value_y_left = this->data[base_x + this->width * base_y];
			value_y_right = this->data[base_x + 1 + this->width * base_y];
		} else {
			value_y_left = (GGen_Height) ((double) this->data[(GGen_Coord) base_x + this->width * base_y] * (1 - remainder_y) + (double) this->data[base_x + this->width * (base_y + 1)] * (remainder_y));
			value_y_right = (GGen_Height) ((double) this->data[(GGen_Coord) base_x + 1 + this->width * base_y] * (1 - remainder_y) + (double)this-> data[base_x + 1 + this->width * (base_y + 1)] * (remainder_y));
		}
	} else {
		value_y_left = value_y_right = (GGen_Height) this->data[(GGen_Coord) base_x + this->width * base_y];
	}

	/* Calculate the interpolated value for horizontal axis */
	if (scale_to_width > this->width) {
		if(remainder_x == 0) return value_y_left;

		return (GGen_Height) ((double) value_y_left * (1 - remainder_x) + (double) value_y_right * (remainder_x));
	} else {
		return ((GGen_Height) this->data[(GGen_Coord) base_x + width * base_y] + value_y_left) /2;
	}
}

void GGen_Data_2D::SetValue(GGen_Coord x, GGen_Coord y, GGen_Height value)
{
	GGen_Script_Assert(x < this->width && y < this->height);
	
	this->data[x + this->width * y] = value;
}

void GGen_Data_2D::SetValueInRect(GGen_Coord x1, GGen_Coord y1, GGen_Coord x2, GGen_Coord y2, GGen_Height value)
{
	GGen_Script_Assert(x2 < this->width && y2 < this->height);
	GGen_Script_Assert(x1 <= x2 && y1 <= y2);

	for (GGen_Coord y = y1; y <= y2; y++) {
		for (GGen_Coord x = x1; x <= x2; x++) {
			this->data[x + this->width * y] = value;
		}
	}

}

void GGen_Data_2D::ScaleTo(GGen_Size new_width, GGen_Size new_height, bool scale_values)
{
	GGen_Script_Assert(new_width >= GGEN_MIN_MAP_SIZE);
	GGen_Script_Assert(new_width <= GGen::GetMaxMapSize());
	GGen_Script_Assert(new_height >= GGEN_MIN_MAP_SIZE);
	GGen_Script_Assert(new_height <= GGen::GetMaxMapSize());

	/* Pick the ratio for values as arithmetic average of horizontal and vertical ratios */
	double ratio = ((double) new_width / (double) this->width + (double) new_height / (double) this->height) / 2.0;

	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[new_width * new_height];

	GGen_Script_Assert(new_data != NULL);

	/* Fill the new array */
	for (GGen_Coord y = 0; y < new_height; y++) {
		for (GGen_Coord x = 0; x < new_width; x++) {
			new_data[x + y * new_width] = scale_values ? (GGen_Height) ((double) this->GetValueInterpolated(x , y, new_width, new_height) * ratio) : this->GetValueInterpolated(x , y, new_width, new_height);
		}
	}

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;
	this->width = new_width;
	this->height = new_height;
	this->length = new_width * new_height;
}

void GGen_Data_2D::ScaleValuesTo(GGen_Height new_min, GGen_Height new_max)
{
	GGen_Script_Assert(new_max > new_min);

	GGen_ExtHeight min = this->Min();
	GGen_ExtHeight max = this->Max() - min;

	GGen_ExtHeight ext_new_min = new_min;
	GGen_ExtHeight ext_new_max = (GGen_ExtHeight) new_max - ext_new_min;

	if (max == 0) return;

	for (GGen_Index i = 0; i < this->length; i++) {
		this->data[i] = (GGen_Height) (ext_new_min + ((GGen_ExtHeight) this->data[i] - min) * ext_new_max / max);
	}
}

void GGen_Data_2D::Scale(double ratio, bool scale_values)
{
	this->ScaleTo((GGen_Size) ((GGen_Size) (double) this->width * ratio), (GGen_Size) ((double) this->height * ratio), scale_values);
}

void GGen_Data_2D::ResizeCanvas(GGen_Size new_width, GGen_Size new_height, GGen_CoordOffset new_zero_x, GGen_CoordOffset new_zero_y)
{
	GGen_Script_Assert(new_width >= GGEN_MIN_MAP_SIZE);
	GGen_Script_Assert(new_width <= GGen::GetMaxMapSize());
	GGen_Script_Assert(new_height >= GGEN_MIN_MAP_SIZE);
	GGen_Script_Assert(new_height <= GGen::GetMaxMapSize());
	GGen_Script_Assert(new_zero_x <= GGen::GetMaxMapSize());
	GGen_Script_Assert(new_zero_x >= -GGen::GetMaxMapSize());
	GGen_Script_Assert(new_zero_y <= GGen::GetMaxMapSize());
	GGen_Script_Assert(new_zero_y >= -GGen::GetMaxMapSize());
	
	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[new_width * new_height];

	GGen_Script_Assert(new_data != NULL);

	for (GGen_Coord y = 0; y < new_height; y++) {
		for (GGen_Coord x = 0; x < new_width; x++) {
			if (y + new_zero_y >= 0 && y + new_zero_y < this->height && x + new_zero_x >= 0 && x + new_zero_x < this->width) {
				new_data[x + y * new_width] = this->data[x + new_zero_x + (y + new_zero_y) * this->width];
			} else {
				new_data[x + y * new_width] = 0;
			}
		}
	}

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;
	this->length = new_width * new_height;
	this->width = new_width;
	this->height = new_height;
}

void GGen_Data_2D::Fill(GGen_Height value)
{
	for (GGen_Index i = 0; i < this->length; i++) {
		this->data[i] = value;
	}
}

void GGen_Data_2D::FillMasked(GGen_Height value, GGen_Data_2D* mask, bool relative)
{
	GGen_Script_Assert(mask != NULL);

	GGen_ExtHeight max = GGEN_UNRELATIVE_CAP;

	if(relative){
		max = mask->Max();
	}

	if(max == 0) return;

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++)	{
			GGen_Height maskValue = mask->GetValueInterpolated(x, y, this->width, this->height);
			this->data[x + y * width] = maskValue * value + (max - maskValue) * this->data[x + y * width];
		}
	}	
}

void GGen_Data_2D::Add(GGen_Height value)
{
	for (GGen_Index i = 0; i < this->length; i++) {
		this->data[i] += value;
	}
}


void GGen_Data_2D::AddMap(GGen_Data_2D* addend)
{
	GGen_Script_Assert(addend != NULL);

	/* Scale the addend as necessary */
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++)	{
			data[x + y * this->width] += addend->GetValueInterpolated(x, y , this->width, this->height);
		}
	}
}

void GGen_Data_2D::ReplaceValue(GGen_Height needle, GGen_Height replace)
{
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++)	{
			if (this->data[x + y * this->width] == needle) {
				this->data[x + y * this->width] = replace;
			}
		}
	}
}

void GGen_Data_2D::AddTo(GGen_Data_2D* addend, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y)
{
	GGen_Script_Assert(addend != NULL);
	GGen_Script_Assert(offset_x > -this->width);
	GGen_Script_Assert(offset_x < this->width);
	GGen_Script_Assert(offset_y > -this->height);
	GGen_Script_Assert(offset_y < this->height);

	/* Walk through the items where the array and the addend with offset intersect */
	for (GGen_Coord y = MAX(0, offset_y); y < MIN(this->height, offset_y + addend->height); y++) {
		for (GGen_Coord x = MAX(0, offset_x); x < MIN(this->width, offset_x + addend->width); x++) {
			this->data[x + y * this->width] += addend->data[(x - offset_x) + (y - offset_y) * addend->width];
		}
	}
}

void GGen_Data_2D::AddMapMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative)
{
	GGen_Script_Assert(addend != NULL);
	GGen_Script_Assert(mask != NULL);
	
	GGen_ExtHeight max = GGEN_UNRELATIVE_CAP;

	if (relative){
		max = mask->Max();
	}
	
	if (max == 0) return;

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			this->data[x + y * this->width] += (GGen_ExtHeight) addend->GetValueInterpolated(x, y, this->width, this->height) * (GGen_ExtHeight) mask->GetValueInterpolated(x, y, this->width, this->height) / max;
		}
	}
}

void GGen_Data_2D::AddMasked(GGen_Height value, GGen_Data_2D* mask, bool relative)
{
	GGen_Script_Assert(mask != NULL);

	GGen_ExtHeight max = GGEN_UNRELATIVE_CAP;

	if(relative){
		max = mask->Max();
	}

	if(max == 0) return;

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++)	{
			this->data[x + y * width] += (GGen_ExtHeight) value * (GGen_ExtHeight) mask->GetValueInterpolated(x, y, this->width, this->height) / max;
		}
	}
}

void GGen_Data_2D::Multiply(double factor)
{
	for (GGen_Index i = 0; i < this->length; i++) {
		this->data[i] = (GGen_Coord) (factor * (double) this->data[i]);
	}
}


void GGen_Data_2D::MultiplyMap(GGen_Data_2D* factor)
{
	GGen_Script_Assert(factor != NULL);

	/* Scale the factor as necessary */
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++)	{
			this->data[x + y * this->width] *= factor->GetValueInterpolated(x, y , this->width, this->height);
		}
	}
}

void GGen_Data_2D::Invert()
{
	for (GGen_Index i = 0; i < length; i++) {
		this->data[i] = -this->data[i];
	}
}


GGen_Height GGen_Data_2D::Min()
{
	GGen_Height temp = GGEN_MAX_HEIGHT;

	for (GGen_Index i = 0; i < this->length; i++) {
		temp = temp > this->data[i] ? this->data[i] : temp;
	}

	return temp;
}

GGen_Height GGen_Data_2D::Max()
{
	GGen_Height temp = GGEN_MIN_HEIGHT;

	for (GGen_Index i = 0; i < this->length; i++) {
		temp = temp < this->data[i] ? this->data[i] : temp;
	}

	return temp;
}

void GGen_Data_2D::Clamp(GGen_Height min, GGen_Height max)
{
	GGen_Script_Assert(max > min);

	for (GGen_Index i = 0; i < this->length; i++) {
		if (this->data[i] > max) {
			this->data[i] = max;
		} else if (data[i] < min) {
			this->data[i] = min;
		}
	}
}

void GGen_Data_2D::CropValues(GGen_Height min, GGen_Height max)
{
	GGen_Script_Assert(max > min);

	for (GGen_Index i = 0; i < this->length; i++) {
		if (this->data[i] > max) {
			this->data[i] = 0;
		} else if (data[i] < min) {
			this->data[i] = 0;
		}
	}
}

void GGen_Data_2D::Union(GGen_Data_2D* victim)
{
	GGen_Script_Assert(victim != NULL);

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {	
			this->data[x + y * width] = MAX(this->data[x + y * this->width], victim->GetValueInterpolated(x, y, this->width, this->height));
		}
	}
}

void GGen_Data_2D::UnionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y)
{
	GGen_Script_Assert(victim != NULL);
	GGen_Script_Assert(offset_x > -this->width);
	GGen_Script_Assert(offset_x < this->width);
	GGen_Script_Assert(offset_y > -this->height);
	GGen_Script_Assert(offset_y < this->height);

	/* Walk through the items where the array and the victim with offset intersect */
	for (GGen_Coord y = MAX(0, offset_y); y < MIN(this->height, offset_y + victim->height); y++) {
		for (GGen_Coord x = MAX(0, offset_x); x < MIN(this->width, offset_x + victim->width); x++) {
			this->data[x + y * width] = MAX(victim->data[(x - offset_x) + (y - offset_y) * victim->width], this->data[x + y * this->width]);
		}
	}
}

void GGen_Data_2D::Intersection(GGen_Data_2D* victim)
{
	GGen_Script_Assert(victim != NULL);

	for(GGen_Coord y = 0; y < this->height; y++) {
		for(GGen_Coord x = 0; x < this->width; x++) {	
			this->data[x + y * this->width] = MIN(this->data[x + y * this->width], victim->GetValueInterpolated(x, y, this->width, this->height));
		}
	}
}

void GGen_Data_2D::IntersectionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y)
{
	GGen_Script_Assert(victim != NULL);
	GGen_Script_Assert(offset_x > -this->width);
	GGen_Script_Assert(offset_x < this->width);
	GGen_Script_Assert(offset_y > -this->height);
	GGen_Script_Assert(offset_y < this->height);

	/* Walk through the items where the array and the addend with offset intersect */
	for (GGen_Coord y = MAX(0, offset_y); y < MIN(this->height, offset_y + victim->height); y++) {
		for (GGen_Coord x = MAX(0, offset_x); x < MIN(this->width, offset_x + victim->width); x++) {
			this->data[x + y * width] = MIN(victim->data[(x - offset_x) + (y - offset_y) * victim->width], this->data[x + y * this->width]);
		}
	}
}

void GGen_Data_2D::Combine(GGen_Data_2D* victim, GGen_Data_2D* mask, bool relative)
{
	GGen_Script_Assert(victim != NULL);
	GGen_Script_Assert(mask != NULL);

	GGen_Height max = GGEN_UNRELATIVE_CAP;

	if(relative){
		max = mask->Max();
	}

	if(max == 0) return;
	
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {	
			this->data[x + y * this->width] = 
				(GGen_ExtHeight) this->data[x + y * this->width] * (GGen_ExtHeight) mask->GetValueInterpolated(x, y, this->width, this->height) / max + 
				(GGen_ExtHeight) victim->GetValueInterpolated(x, y, this->width, this->height) * (GGen_ExtHeight) (max - mask->GetValueInterpolated(x, y, this->width, this->height))/ max;
		}
	}
}

void GGen_Data_2D::Abs()
{
	for (GGen_Index i = 0; i < this->length; i++) {
		if (this->data[i] < 0) {
			this->data[i] = -this->data[i];
		}
	}
}

void GGen_Data_2D::Project(GGen_Data_1D* profile, GGen_Direction direction)
{
	GGen_Script_Assert(profile != NULL);

	if (direction == GGEN_HORIZONTAL) {
		for(GGen_Coord y = 0; y < this->height; y++) {
			for(GGen_Coord x = 0; x < this->width; x++) {		
				this->data[x + y * this->width] = profile->GetValueInterpolated(y, this->height);
			}
		}
	} else {
		for (GGen_Coord y = 0; y < this->height; y++) {
			for (GGen_Coord x = 0; x < this->width; x++) {		
				this->data[x + y * this->width] = profile->GetValueInterpolated(x, this->width);
			}
		}
	}
}

GGen_Data_1D* GGen_Data_2D::GetProfile(GGen_Direction direction, GGen_Coord coordinate)
{
	GGen_Script_Assert(direction == GGEN_VERTICAL || coordinate < this->width);
	GGen_Script_Assert(direction == GGEN_HORIZONTAL || coordinate < this->height);
	
	GGen_Data_1D* output;
	
	if (direction == GGEN_HORIZONTAL) {
		output = new GGen_Data_1D(this->width, 0);
		
		for  (GGen_Coord x = 0; x < this->width; x++) {		
			output->data[x] = this->data[x + coordinate * this->width];
		}
	} else {
		output = new GGen_Data_1D(this->height, 0);
	
		for (GGen_Coord y = 0; y < this->height; y++) {
			output->data[y] = this->data[coordinate + y * this->width];
		}
	}
	
	return output;
}

void GGen_Data_2D::Shift(GGen_Data_1D* profile, GGen_Direction direction, GGen_Overflow_Mode mode)
{
	GGen_Script_Assert(profile != NULL);

	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[this->length];

	GGen_Script_Assert(new_data != NULL);

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {		
			if (direction == GGEN_VERTICAL) {
				GGen_Height distance = profile->GetValueInterpolated(x, this->width);

				/* Some values can be just plainly shifted */
				if ((distance >= 0 && y < this->height - distance) || (distance <= 0 && (signed) y >= -distance)) {
					new_data[x + (y + distance) * this->width] = this->data[x + y * this->width];
				} else if(distance >= 0) {
					/* Some must go through the upper "border" */
					if (mode == GGEN_CYCLE) {
						new_data[x + (y - this->height + distance) * this->width] = this->data[x + y * this->width];
					} else if (mode == GGEN_DISCARD_AND_FILL) {
						new_data[x + (y - this->height + distance) * this->width] = this->data[x];
					}
				} else {
					/* And some must go through the bottom "border" */
					if (mode == GGEN_CYCLE) {
						new_data[x + (y + this->height + distance) * this->width] = this->data[x + y * this->width];
					} else if ( mode == GGEN_DISCARD_AND_FILL) {
						new_data[x + (y + this->height + distance) * this->width] = this->data[x + (this->height - 1) * this->width];
					}
				}					
			} else { /* GGEN_HORIZONTAL */
				GGen_Height distance = profile->GetValueInterpolated(y, this->height);

				/* Some values can be just plainly shifted */
				if ((distance >= 0 && x < this->width - distance) || (distance <= 0 && (signed) x >= -distance)) {
					new_data[x + distance + y * this->width] = this->data[x + y * this->width];
				} else if (distance >= 0) {
					/* Some must go through the right "border" */
					if (mode == GGEN_CYCLE) {
						new_data[x - this->width + distance + y * this->width] = this->data[x + y * this->width];
					} else if (mode == GGEN_DISCARD_AND_FILL) {
						new_data[x - this->width + distance + y * this->width] = this->data[y * this->width];
					}
				} else {
					/* And some must go through the left "border" */
					if (mode == GGEN_CYCLE) {
						new_data[x + this->width + distance + y * this->width] = this->data[x + y * this->width];
					} else if (mode == GGEN_DISCARD_AND_FILL) {
						new_data[x + this->width + distance + y * this->width] = this->data[this->width - 1 + y * this->width];
					}
				}					
			}
		}
	}

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;
}

void GGen_Data_2D::GradientFromProfile(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Data_1D* pattern, bool fill_outside)
{
	GGen_Script_Assert(pattern != NULL);
	GGen_Script_Assert(from_x < this->width);
	GGen_Script_Assert(from_y < this->height);
	GGen_Script_Assert(to_x < this->width);
	GGen_Script_Assert(to_y < this->height);

	GGen_ExtExtHeight target_x = to_x - from_x;
	GGen_ExtExtHeight target_y = to_y - from_y;
	
	/* Width of the gradient strip */
	double max_dist = sqrt((double) (ABS(to_x - from_x) * ABS(to_x - from_x) + ABS(to_y - from_y) * ABS(to_y - from_y)));

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			GGen_ExtExtHeight point_x = x - from_x;
			GGen_ExtExtHeight point_y = y - from_y;

			/* Get the point on the gradient vector (vector going through both starting and target point) to which is the current point closest */
			GGen_ExtExtHeight cross_x = (target_x * (target_x * point_x + target_y * point_y)) / (target_x * target_x + target_y * target_y);
			GGen_ExtExtHeight cross_y = (target_y * (target_x * point_x + target_y * point_y)) / (target_x * target_x + target_y * target_y);		
		
			/* Calculate the distance from the "from" point to the intersection with gradient vector */
			double distance = sqrt((double) (cross_x * cross_x + cross_y * cross_y));
			
			/* Distance from  the intersection point to the target point */
			double reverse_distance = sqrt((double) ( ABS(target_x - cross_x) * ABS(target_x - cross_x) + ABS(target_y - cross_y) * ABS(target_y - cross_y) ));
			
			/* Apply it to the array data */
			if(distance <= max_dist && reverse_distance <= max_dist) {
				this->data[x + this->width * y] = pattern->GetValueInterpolated((GGen_Distance) distance, (GGen_Size) max_dist + 1);
			} else if (fill_outside && reverse_distance < distance) {
				this->data[x + this->width * y] = pattern->GetValue(pattern->length - 1);
			} else if(fill_outside) {
				this->data[x + this->width * y] = pattern->GetValue(0);
			}
		}
	}
}

void GGen_Data_2D::Gradient(GGen_Coord from_x, GGen_Coord from_y, GGen_Coord to_x, GGen_Coord to_y, GGen_Height from_value, GGen_Height to_value, bool fill_outside)
{
	GGen_Script_Assert(from_x < this->width);
	GGen_Script_Assert(from_y < this->height);
	GGen_Script_Assert(to_x < this->width);
	GGen_Script_Assert(to_y < this->height);

	/* Call the profile gradient with linear profile */
	
	GGen_Data_1D temp(2, 0);
	temp.SetValue(0, from_value);
	temp.SetValue(1, to_value);

	this->GradientFromProfile(from_x, from_y, to_x, to_y, &temp, fill_outside);
}

void GGen_Data_2D::RadialGradientFromProfile(GGen_Coord center_x, GGen_Coord center_y, GGen_Distance radius, GGen_Data_1D* pattern, bool fill_outside)
{
	GGen_Script_Assert(pattern != NULL);
	GGen_Script_Assert(center_x < this->width);
	GGen_Script_Assert(center_y < this->height);

	GGen_Script_Assert(radius > 0);
	GGen_Script_Assert(pattern != NULL);

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			GGen_Distance distance = (GGen_Distance) sqrt((double) (ABS(x - center_x) * ABS(x - center_x) + ABS(y - center_y) * ABS(y - center_y)));
		 
			if (distance < radius) {
				this->data[x + this->width * y] = pattern->GetValueInterpolated(distance, radius);
			} else if (fill_outside) {
				this->data[x + this->width * y] = pattern->GetValue(pattern->length - 1);
			}
		}
	}
}

void GGen_Data_2D::RadialGradient(GGen_Coord center_x, GGen_Coord center_y, GGen_Coord radius, GGen_Height min, GGen_Height max, bool fill_outside)
{
	GGen_Script_Assert(center_x < this->width);
	GGen_Script_Assert(center_y < this->height);

	GGen_Script_Assert(radius > 0);	
	
	GGen_ExtExtHeight rel_max = max - min;

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			GGen_Distance distance = (GGen_Distance) sqrt((double) (ABS(x - center_x) * ABS(x - center_x) + ABS(y - center_y) * ABS(y - center_y)));

			if (distance < radius) {
				this->data[x + this->width * y] = (GGen_Height) ((GGen_ExtExtHeight) min + rel_max * (GGen_ExtExtHeight) distance / (GGen_ExtExtHeight) radius);
			} else if (fill_outside) {
				this->data[x + this->width * y] = (GGen_Height) ((GGen_ExtHeight) min + rel_max);
			}
		}
	}
}

// Returns value usable for interpolation from give coordinate. The coordinate might be outside the map.
double Noise_GetGridPoint(GGen_Data_2D* map, GGen_CoordOffset x, GGen_CoordOffset y, GGen_Height* verticalOverflowBuffer, GGen_Height* horizontalOverflowBuffer){
	// The value is outside the right border of the map - mirror the coordinate, so a realistically behaving value is used.
	if(x < 0){
		x = -x;
	}

	// The value is outside the right border - use value from the overflow buffer.
	else if(x >= map->width){
		return verticalOverflowBuffer[MAX(MIN(y, map->height), 0)];
	}

	// The value is outside the top border of the map - mirror the coordinate, so a realistically behaving value is used.
	if(y < 0){
		y = -y;
	}
	
	// The value is outside the bottom border - use value from the overflow buffer.
	else if(y >= map->height){
		return horizontalOverflowBuffer[x];
	}

	// The point is within the map.
	return map->data[x + map->width * y];
}

GGen_Height Noise_BicubicInterpolation(double p[4][4]){
	// Prepare coefficients for the bicubic polynomial.
	double a00 = p[1][1];
	double a01 = -.5*p[1][0] + .5*p[1][2];
	double a02 = p[1][0] - 2.5*p[1][1] + 2*p[1][2] - .5*p[1][3];
	double a03 = -.5*p[1][0] + 1.5*p[1][1] - 1.5*p[1][2] + .5*p[1][3];
	double a10 = -.5*p[0][1] + .5*p[2][1];
	double a11 = .25*p[0][0] - .25*p[0][2] - .25*p[2][0] + .25*p[2][2];
	double a12 = -.5*p[0][0] + 1.25*p[0][1] - p[0][2] + .25*p[0][3] + .5*p[2][0] - 1.25*p[2][1] + p[2][2] - .25*p[2][3];
	double a13 = .25*p[0][0] - .75*p[0][1] + .75*p[0][2] - .25*p[0][3] - .25*p[2][0] + .75*p[2][1] - .75*p[2][2] + .25*p[2][3];
	double a20 = p[0][1] - 2.5*p[1][1] + 2*p[2][1] - .5*p[3][1];
	double a21 = -.5*p[0][0] + .5*p[0][2] + 1.25*p[1][0] - 1.25*p[1][2] - p[2][0] + p[2][2] + .25*p[3][0] - .25*p[3][2];
	double a22 = p[0][0] - 2.5*p[0][1] + 2*p[0][2] - .5*p[0][3] - 2.5*p[1][0] + 6.25*p[1][1] - 5*p[1][2] + 1.25*p[1][3] + 2*p[2][0] - 5*p[2][1] + 4*p[2][2] - p[2][3] - .5*p[3][0] + 1.25*p[3][1] - p[3][2] + .25*p[3][3];
	double a23 = -.5*p[0][0] + 1.5*p[0][1] - 1.5*p[0][2] + .5*p[0][3] + 1.25*p[1][0] - 3.75*p[1][1] + 3.75*p[1][2] - 1.25*p[1][3] - p[2][0] + 3*p[2][1] - 3*p[2][2] + p[2][3] + .25*p[3][0] - .75*p[3][1] + .75*p[3][2] - .25*p[3][3];
	double a30 = -.5*p[0][1] + 1.5*p[1][1] - 1.5*p[2][1] + .5*p[3][1];
	double a31 = .25*p[0][0] - .25*p[0][2] - .75*p[1][0] + .75*p[1][2] + .75*p[2][0] - .75*p[2][2] - .25*p[3][0] + .25*p[3][2];
	double a32 = -.5*p[0][0] + 1.25*p[0][1] - p[0][2] + .25*p[0][3] + 1.5*p[1][0] - 3.75*p[1][1] + 3*p[1][2] - .75*p[1][3] - 1.5*p[2][0] + 3.75*p[2][1] - 3*p[2][2] + .75*p[2][3] + .5*p[3][0] - 1.25*p[3][1] + p[3][2] - .25*p[3][3];
	double a33 = .25*p[0][0] - .75*p[0][1] + .75*p[0][2] - .25*p[0][3] - .75*p[1][0] + 2.25*p[1][1] - 2.25*p[1][2] + .75*p[1][3] + .75*p[2][0] - 2.25*p[2][1] + 2.25*p[2][2] - .75*p[2][3] - .25*p[3][0] + .75*p[3][1] - .75*p[3][2] + .25*p[3][3];

	// Calculate value of the bicubic polynomial.
	double value = (a00 + a01 * 0.5 + a02 * 0.5 * 0.5 + a03 * 0.5 * 0.5 * 0.5) +
		(a10 + a11 * 0.5 + a12 * 0.5 * 0.5 + a13 * 0.5 * 0.5 * 0.5) * 0.5 +
		(a20 + a21 * 0.5 + a22 * 0.5 * 0.5 + a23 * 0.5 * 0.5 * 0.5) * 0.5 * 0.5 +
		(a30 + a31 * 0.5 + a32 * 0.5 * 0.5 + a33 * 0.5 * 0.5 * 0.5) * 0.5 * 0.5 * 0.5;

	// Check for overflows.
	GGen_Script_Assert(value >= GGEN_MIN_HEIGHT && value <= GGEN_MAX_HEIGHT);

	return (GGen_Height) value;
}

void GGen_Data_2D::Noise(GGen_Size minFeatureSize, GGen_Size maxFeatureSize, GGen_Amplitudes* amplitudes)
{
	GGen_Script_Assert(amplitudes != NULL);	
	GGen_Script_Assert(minFeatureSize > 0);
	GGen_Script_Assert(minFeatureSize <= GGen::GetMaxMapSize());
	GGen_Script_Assert(maxFeatureSize > 0);
	GGen_Script_Assert(maxFeatureSize <= GGen::GetMaxMapSize());

	/* This function uses the diamond-square mid-point displacement algorithm with bicubic interpolation. */

	// Reset the map array, it will be filled with completely new values.
	this->Fill(0);	

	/* The diamond-square algorithm by definition works on square maps only with side length equal to power  
	 * of two plus one. But only two rows/columns of points are enough to interpolate the points near the right 
	 * and bottom borders (top and bottom borders are aligned with the grid, so they can be interpolated in much
	 * simpler fashion) - thanks to the fact that this is mid-point displacement algorithm. The vertical buffer
	 * stands for any points to outside the right border, the horizontal buffer then analogously serves as all 
	 * points below the bottom border. The points outside both right and bottom border are represented by last
	 * point in the vertical buffer. */	
	GGen_Height* verticalOverflowBuffer = new GGen_Height[this->height + 1];
	GGen_Height* horizontalOverflowBuffer = new GGen_Height[this->width];

	// The supplied wave length is likely not a power of two. Convert the number to the nearest lesser power of two.
	unsigned waveLengthLog2 = (unsigned) GGen_log2(maxFeatureSize);
	GGen_Size waveLength = 1 << waveLengthLog2;
	GGen_Height amplitude = amplitudes->data[waveLengthLog2];

	// Generate the initial seed values (square corners).
	for(GGen_Coord y = 0; ; y += waveLength){
		if(y < this->height){
			for(GGen_Coord x = 0; ; x += waveLength){
				if(x < this->width){
					this->data[x + this->width * y] = GGen_Random<GGen_Height>(-amplitude, amplitude);
				}
				else{
					verticalOverflowBuffer[y] = GGen_Random<GGen_Height>(-amplitude, amplitude);
					break;
				}
			}
		}
		else {
			for(GGen_Coord x = 0; ; x += waveLength){
				if(x < this->width){
					horizontalOverflowBuffer[x] = GGen_Random<GGen_Height>(-amplitude, amplitude);
				}
				else{
					verticalOverflowBuffer[this->height] = GGen_Random<GGen_Height>(-amplitude, amplitude);
					break;
				}
			}
			break;
		}
	}

	amplitude = amplitudes->data[waveLengthLog2 - 1];

	// Keep interpolating until there are uninterpolated tiles..
	while(waveLength > 1){
		GGen_Size halfWaveLength = waveLength / 2;

		// The square step - put a randomly generated point into center of each square.
		bool breakY = false;
		for(GGen_Coord y = halfWaveLength; breakY == false; y += waveLength){			
			for(GGen_Coord x = halfWaveLength; ; x += waveLength){
				// Prepare the 4x4 value matrix for bicubic interpolation.
				GGen_CoordOffset x0 = (int) x - (int) halfWaveLength - (int) waveLength;
				GGen_CoordOffset x1 = (int) x - (int) halfWaveLength;
				GGen_CoordOffset x2 = (int) x + (int) halfWaveLength;
				GGen_CoordOffset x3 = (int) x + (int) halfWaveLength + (int) waveLength;

				GGen_CoordOffset y0 = (int) y - (int) halfWaveLength - (int) waveLength;
				GGen_CoordOffset y1 = (int) y - (int) halfWaveLength;
				GGen_CoordOffset y2 = (int) y + (int) halfWaveLength;
				GGen_CoordOffset y3 = (int) y + (int) halfWaveLength + (int) waveLength;

				double data[4][4] = {
					{
						Noise_GetGridPoint(this, x0, y0, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x1, y0, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x2, y0, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x3, y0, verticalOverflowBuffer, horizontalOverflowBuffer),
					},
					{
						Noise_GetGridPoint(this, x0, y1, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x1, y1, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x2, y1, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x3, y1, verticalOverflowBuffer, horizontalOverflowBuffer),
					},
					{
						Noise_GetGridPoint(this,x0, y2, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x1, y2, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x2, y2, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x3, y2, verticalOverflowBuffer, horizontalOverflowBuffer),
					},
					{
						Noise_GetGridPoint(this, x0, y3, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x1, y3, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x2, y3, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x3, y3, verticalOverflowBuffer, horizontalOverflowBuffer),
					},
				};

				// Interpolate!
				GGen_Height interpolatedHeight = Noise_BicubicInterpolation(data) + (waveLength < minFeatureSize ? 0 : GGen_Random<GGen_ExtHeight>(-amplitude, amplitude));

				// Place the value into one of the overflow buffers, if it is outside the map.
				if(x >= this->width){
					verticalOverflowBuffer[MIN(y, this->height)] = interpolatedHeight;
					break;
				}
				else if(y >= this->height) {
					horizontalOverflowBuffer[x] = interpolatedHeight;
					breakY = true;
				}
				else{
					this->data[x + this->width * y] = interpolatedHeight;
				}
			}
		}

		// The diamond step - add point into middle of each diamond so each square from the square step is composed of 4 smaller squares.
		breakY = false;
		bool evenRow = true;
		for(GGen_Coord y = 0; breakY == false; y += halfWaveLength){			
			for(GGen_Coord x = evenRow ? halfWaveLength : 0; ; x += waveLength){							
				// Prepare the 4x4 value matrix for bicubic interpolation (this time rotated by 45 degrees).
				GGen_CoordOffset x0 = (int) x - (int) halfWaveLength - (int) waveLength;
				GGen_CoordOffset x1 = (int) x - (int) waveLength;
				GGen_CoordOffset x2 = (int) x - (int) halfWaveLength;
				GGen_CoordOffset x3 = (int) x;
				GGen_CoordOffset x4 = (int) x + (int) halfWaveLength;
				GGen_CoordOffset x5 = (int) x + (int) waveLength;
				GGen_CoordOffset x6 = (int) x + (int) halfWaveLength + (int) waveLength;

				GGen_CoordOffset y0 = (int) y - (int) halfWaveLength - (int) waveLength;
				GGen_CoordOffset y1 = (int) y - (int) waveLength;
				GGen_CoordOffset y2 = (int) y - (int) halfWaveLength;
				GGen_CoordOffset y3 = (int) y;
				GGen_CoordOffset y4 = (int) y + (int) halfWaveLength;
				GGen_CoordOffset y5 = (int) y + (int) waveLength;
				GGen_CoordOffset y6 = (int) y + (int) halfWaveLength + (int) waveLength;

				double data[4][4] = {
					{
						Noise_GetGridPoint(this, x0, y3, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this,x1, y4, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x2, y5, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x3, y6, verticalOverflowBuffer, horizontalOverflowBuffer),
					},
					{
						Noise_GetGridPoint(this, x1, y2, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x2, y3, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x3, y4, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x4, y5, verticalOverflowBuffer, horizontalOverflowBuffer),
					},
					{
						Noise_GetGridPoint(this, x2, y1, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x3, y2, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x4, y3, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x5, y4, verticalOverflowBuffer, horizontalOverflowBuffer),
					},
					{
						Noise_GetGridPoint(this, x3, y0, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x4, y1, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x5, y2, verticalOverflowBuffer, horizontalOverflowBuffer),
						Noise_GetGridPoint(this, x6, y3, verticalOverflowBuffer, horizontalOverflowBuffer),
					},
				};

				// Interpolate!
				GGen_Height interpolatedHeight = Noise_BicubicInterpolation(data) + (waveLength < minFeatureSize ? 0 : GGen_Random<GGen_ExtHeight>(-amplitude, amplitude));

				// Place the value into one of the overflow buffers, if it is outside the map.
				if(x >= this->width){
					verticalOverflowBuffer[MIN(y, this->height)] = interpolatedHeight;
					break;
				}
				else if(y >= this->height) {
					horizontalOverflowBuffer[x] = interpolatedHeight;
					breakY = true;
				}
				else{
					this->data[x + this->width * y] = interpolatedHeight;
				}
			}

			// The X coordinates are shifted by waveLength/2 in even rows.
			evenRow = !evenRow;
		}

		// Decrease the wave length and amplitude.
		waveLength /= 2;
		if(waveLength > 1){						
			amplitude = amplitudes->data[GGen_log2(waveLength / 2)];
		}
	}

	delete [] verticalOverflowBuffer;
	delete [] horizontalOverflowBuffer;

	return;
} 

// NOT REALLY FINISHED!! NEEDS A LOT OF POLISH!!!
void GGen_Data_2D::VoronoiNoise(GGen_Size cell_size, uint8 points_per_cell, GGen_Voronoi_Noise_Mode mode)
{
	GGen_Script_Assert(cell_size > 2);
	GGen_Script_Assert(points_per_cell >= 1);

	#define VORONOINOISE_GET_POINT(x, y, i) points[i + (x) * points_per_cell + (y) * num_cells_x * points_per_cell]
	
	struct Point{
		GGen_Coord x, y;
	};
	
	uint16 num_cells_x = (uint16) ceil((double) this->width / (double) cell_size);
	uint16 num_cells_y = (uint16) ceil((double) this->height / (double) cell_size);
	int cell_width = cell_size;
	int cell_height = cell_size;
	
	GGen_ExtExtHeight max_dist = 2 * cell_width * cell_width + cell_height * cell_height;
	
	uint16 num_points_current_cell = 1;

	GGen_Size overlap_x;
	GGen_Size overlap_y;
	if (this->width % cell_size == 0) {
		num_cells_x++;
		overlap_x = cell_size / 2;
		
	} else {
		overlap_x = (this->width % cell_size) / 2;
		num_cells_x++;
	}

	if (this->height % cell_size == 0) {
		num_cells_y++;
		overlap_y = cell_size / 2;
	} else {
		overlap_y = (this->height % cell_size) / 2;
		num_cells_y++;
	}

	Point* points = new Point[num_cells_x * num_cells_y * points_per_cell];
	
	//Point points[100];

	/* Distribute the points into cells */
	for (int y = 0; y < num_cells_y; y++) {
		for (int x = 0; x < num_cells_x; x++) {
			for (int i = 0; i < points_per_cell; i++) {
				if (x == 0) {
					VORONOINOISE_GET_POINT(x, y, i).x = GGen_Random<int>(0, overlap_x);
				} else if (x < num_cells_x - 1) {
					VORONOINOISE_GET_POINT(x, y, i).x = overlap_x + (x - 1) * cell_width + GGen_Random<int>(0, cell_width);
				} else {
					VORONOINOISE_GET_POINT(x, y, i).x = overlap_x + (x - 1) * cell_width + GGen_Random<int>(0, overlap_x);
				}

				if (y == 0) {
					VORONOINOISE_GET_POINT(x, y, i).y = GGen_Random<int>(0, overlap_y);
				} else if (y < num_cells_y - 1) {
					VORONOINOISE_GET_POINT(x, y, i).y = overlap_y + (y - 1) * cell_height + GGen_Random<int>(0, cell_height);
				} else {
					VORONOINOISE_GET_POINT(x, y, i).y = overlap_y + (y - 1) * cell_height + GGen_Random<int>(0, overlap_y);
				}

			}	
		}
	}
	
	/* We will first collect the points from surrounding cells of each pixel and then work them individually.
	 * Since we know the number of points in each cell, we can hold only one pointer in this array - to the first point
	 * in the cell (so there will be only up to 9 pointers in the array). */
	Point* points_waiting[9];
	
	for(GGen_Coord y = 0; y < height; y++){
		for(GGen_Coord x = 0; x < width; x++){
			/* We will always use at least  one cell (the current one) */
			uint8 num_points_waiting = 1;
			
			/* Coordinates in the cell grid */
			GGen_Coord cell_x = x / cell_width;
			GGen_Coord cell_y = y / cell_height;

			if (x < overlap_x) {
				cell_x = 0;
			} else if (x < this->width - overlap_x) {
				cell_x = 1 + ((x - overlap_x) / cell_width);
			} else {
				cell_x = num_cells_x - 1;
			}
		
			if (y < overlap_y) {
				cell_y = 0;
			} else if (y < this->height - overlap_y) {
				cell_y = 1 + ((y - overlap_y) / cell_height);
			} else {
				cell_y = num_cells_y - 1;
			}

			/* Distances to the nearest and second nearest point */
			GGen_ExtExtHeight current_min_dist = max_dist - 1;
			GGen_ExtExtHeight current_second_min_dist = max_dist - 1;
			
			/* The current cell */
			points_waiting[0] = &VORONOINOISE_GET_POINT(cell_x, cell_y, 0);
		
			/* Collect points from the surrounding cells */
			if(cell_y > 0 && cell_x > 0)							{points_waiting[num_points_waiting] = &VORONOINOISE_GET_POINT(cell_x - 1, cell_y - 1, 0); ++num_points_waiting;}
			if(cell_y > 0)											{points_waiting[num_points_waiting] = &VORONOINOISE_GET_POINT(cell_x,	 cell_y - 1, 0); ++num_points_waiting;}
			if(cell_y > 0 && cell_x < num_cells_x - 1)				{points_waiting[num_points_waiting] = &VORONOINOISE_GET_POINT(cell_x + 1, cell_y - 1, 0); ++num_points_waiting;}
			if(cell_x > 0)											{points_waiting[num_points_waiting] = &VORONOINOISE_GET_POINT(cell_x - 1, cell_y	   , 0); ++num_points_waiting;}
			if(cell_x < num_cells_x - 1)							{points_waiting[num_points_waiting] = &VORONOINOISE_GET_POINT(cell_x + 1, cell_y	   , 0); ++num_points_waiting;}
			if(cell_x > 0 && cell_y < num_cells_y - 1)				{points_waiting[num_points_waiting] = &VORONOINOISE_GET_POINT(cell_x - 1, cell_y + 1, 0); ++num_points_waiting;}
			if(cell_y < num_cells_y - 1)							{points_waiting[num_points_waiting] = &VORONOINOISE_GET_POINT(cell_x	   , cell_y + 1, 0); ++num_points_waiting;}
			if(cell_x < num_cells_x - 1 && cell_y < num_cells_y - 1){points_waiting[num_points_waiting] = &VORONOINOISE_GET_POINT(cell_x + 1, cell_y + 1, 0); ++num_points_waiting;}
			
			/* Find the closest point */
			for(int i = 0; i < num_points_waiting; i++){
				for(int j = 0; j < points_per_cell; j++){
					Point* current_point = points_waiting[i] + j;
					
					double current_distance = (double) (x - current_point->x) * (double) (x - current_point->x) + (double) (y - current_point->y) * (double) (y - current_point->y);
					
					/* Is the current point closer than the nearest one? */
					if(current_distance <= current_min_dist){
						/* The current closest point will become the second closest */
						current_second_min_dist = current_min_dist;

						current_min_dist = (GGen_ExtExtHeight) current_distance;
					}

					/* The current point is closer than the second closest one, but further than the nearest one -> it can still become #2 */
					if(current_distance > current_min_dist && current_distance < current_second_min_dist){
						current_second_min_dist = (GGen_ExtExtHeight) current_distance;
					}
				}
			}
			
			/* Calculate value for current point from the distances to the two closest points */
			if(mode == GGEN_RIDGES) data[x + y * width] = (GGen_Height) ((-current_min_dist + current_second_min_dist) * GGEN_MAX_HEIGHT / max_dist);
			else if(mode == GGEN_BUBBLES) data[x + y * width] = (GGen_Height) (current_min_dist * GGEN_MAX_HEIGHT / max_dist);
		}
	}

	#undef VORONOINOISE_GET_POINT
}

/*
 * Shifts the array values so given percentage of it is under zero (zero represents the water level).
 * @param percentage of the map to be flooded
 */
void GGen_Data_2D::Flood(double water_amount)
{
	GGen_Script_Assert(water_amount < 1 && water_amount > 0);

	GGen_Index target = (GGen_Index) (water_amount * (double) this->length);

	GGen_Height min = this->Min();
	GGen_Height max = this->Max();

	// Use the binary search algorithm to find the correct water level
	GGen_Height middle;
	while (max - min > 1) {
		middle = min + (max - min) / 2;

		GGen_Index amount = 0;
		for (GGen_Index i = 0; i < this->length; i++) {
			if(this->data[i] >= middle) amount++;
		}

		if (amount > target) {
			min = middle;
		} else {
			max = middle;
		}
	}

	/* Shift the heights so given portion of the array is under zero */
	this->Add(-middle);
}

void GGen_Data_2D::Smooth(GGen_Distance radius)
{
	GGen_Script_Assert(radius > 0);
	
	this->SmoothDirection(radius, GGEN_HORIZONTAL);
	this->SmoothDirection(radius, GGEN_VERTICAL);
}

void GGen_Data_2D::SmoothDirection(GGen_Distance radius, GGen_Direction direction)
{
	GGen_Script_Assert(radius > 0);
	
	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[this->length];

	GGen_Script_Assert(new_data != NULL);

	if (direction == GGEN_HORIZONTAL) {
		for (GGen_Coord y = 0; y < height; y++) {
			/* Prefill the window with value of the left edge + n leftmost values (where n is radius) */
			GGen_Size window_size = radius * 2 + 1;
			GGen_ExtHeight window_value = this->data[this->width * y] * radius;

			for (GGen_Distance x = 0; x < radius; x++) {
				window_value += this->data[x + this->width * y];
			}

			/* In every step shift the window one tile to the right  (= subtract its leftmost cell and add
			value of rightmost + 1). i represents position of the central cell of the window. */
			for (GGen_Coord x = 0; x < this->width; x++) {
				/* If the window is approaching right border, use the rightmost value as fill. */
				if (x < radius) {
					window_value += this->data[x + radius + this->width * y] - this->data[this->width * y];
				} else if (x + radius < this->width) {
					window_value += this->data[x + radius + this->width * y] - this->data[x - radius + this->width * y];
				} else {
					window_value += this->data[this->width - 1 + this->width * y] - this->data[x - radius + this->width * y];
				}

				/* Set the value of current tile to arithmetic average of window tiles. */
				new_data[x + this->width * y] = window_value / window_size;
			}
		}
	} else { /* vertical */
		for (GGen_Coord x = 0; x < this->width; x++) {
			/* Prefill the window with value of the left edge + n topmost values (where n is radius) */
			GGen_Size window_size = radius * 2 + 1;
			GGen_ExtHeight window_value = this->data[x] * radius;

			for (GGen_Distance y = 0; y < radius; y++) {
				window_value += this->data[x + y * this->width];
			}

			/* In every step shift the window one tile to the bottom  (= subtract its topmost cell and add
			value of bottommost + 1). i represents position of the central cell of the window. */
			for (GGen_Coord y = 0; y < this->height; y++) {
				/* If the window is approaching right border, use the rightmost value as fill. */
				if (y < radius) {
					window_value += this->data[x + (y + radius) * this->width] - this->data[x];
				} else if (y + radius < height) {
					window_value += this->data[x + (y + radius) * this->width] - this->data[x + (y - radius) * this->width];
				} else {
					window_value += this->data[x + (this->height - 1) * this->width] - this->data[x + (y - radius) * this->width];
				}

				/* Set the value of current tile to arithmetic average of window tiles. */
				new_data[x + this->width * y] = window_value / window_size;
			}
		}
	}	
	

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;	
}

void GGen_Data_2D::Pattern(GGen_Data_2D* pattern)
{
	GGen_Script_Assert(pattern != NULL);
	
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			this->data[x + y * this->width] = pattern->data[ y % pattern->width + (x % pattern->height) * pattern->height];
		}
	}
}

void GGen_Data_2D::ReturnAs(const GGen_String &name)
{
	if(GGen::GetInstance()->return_callback == NULL) {
		GGen::GetInstance()->ThrowMessage(GGen_Const_String("The script returned a named map, but return handler was not defined"), GGEN_WARNING);
		return;
	}

	/* Call the defined return callback */
	GGen::GetInstance()->return_callback(name, this->data, this->width, this->height);
}

void GGen_Data_2D::Monochrome(GGen_Height threshold)
{
	for (GGen_Index i = 0; i < this->length; i++) {
		this->data[i] = this->data[i] > threshold ? 1 : 0;
	}	
}

void GGen_Data_2D::SelectValue(GGen_Height value)
{
	for (GGen_Index i = 0; i < this->length; i++) {
		this->data[i] = this->data[i] == value ? 1 : 0;
	}	
}


void GGen_Data_2D::SlopeMap()
{
	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[this->length];

	GGen_Script_Assert(new_data != NULL);
	GGen_Script_Assert(this->width > 2 && this->height > 2);

	for (GGen_Coord y = 1; y < this->height - 1; y++) {
		for (GGen_Coord x = 1; x < this->width - 1; x++) {		
			new_data[x + y * this->width] = 
				MAX(
					ABS(this->data[x + y * this->width - 1] - this->data[x + y * this->width + 1]), 
					ABS(this->data[x + y * this->width - this->width] - this->data[x + y * this->width + this->width])
				);
		}
	}

	/* Fix the corners */
	new_data[0] =  new_data[this->width + 1];
	new_data[this->width - 1] = new_data[2 * this->width - 2];
	new_data[this->length - this->width] = new_data[this->length - 2 * this->width + 1];
	new_data[length - 1] = new_data[length - width - 2];

	/* Upper border */
	for (GGen_Coord x = 1; x < this->width - 1; x++) {
		new_data[x] = new_data[x + this->width];
	}

	/* Bottom corner */
	for (GGen_Coord x = 1; x < this->width - 1; x++) {
		new_data[this->length - this->width + x] = new_data[this->length - 2 * this->width + x];
	}

	/* Left border */
	for (GGen_Coord y = 1; y < this->height - 1; y++) {
		new_data[y * this->width] = new_data[y * this->width + 1];
	}

	/* Right border */
	for (GGen_Coord y = 1; y < this->height - 1; y++) {
		new_data[y * this->width + this->width - 1] = new_data[y * this->width + this->width - 2];
	}

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;	
}

void GGen_Data_2D::Scatter(bool relative)
{
	GGen_Height max = GGEN_UNRELATIVE_CAP;

	if(relative){
		max = this->Max();
	}

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {		
			this->data[x + y * this->width] = GGen_Random<GGen_Height>(0, max) > this->data[x + y * this->width] ? 0 : 1;
		}
	}
}

void GGen_Data_2D::TransformValues(GGen_Data_1D* profile, bool relative)
{
	GGen_Script_Assert(profile != NULL);

	GGen_Height min = 0;
	GGen_Height max = GGEN_UNRELATIVE_CAP;

	if(relative){
		max = this->Max();
	}

	/* Transform the values */
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {	
			if (this->data[x + y * this->width] > 0) {
				GGen_Height height = this->data[x + this->width * y];
				this->data[x + y * this->width] = x > 0 ? profile->GetValueInterpolated(height, max + 1) : 0;
			}
		}
	}
}

void GGen_Data_2D::NormalizeDirection(GGen_Direction direction, GGen_Normalization_Mode mode)
{
	if (direction == GGEN_HORIZONTAL) {
		for (GGen_Coord y = 0; y < this->height; y++){
			GGen_Height last = this->data[y * this->width];

			for (GGen_Coord x = 0; x < this->width; x++) {
				if (mode == GGEN_ADDITIVE && this->data[x + y * this->width] > last + 1) {
					this->data[x + y * this->width] = last + 1;
				} else if (mode == GGEN_SUBSTRACTIVE && this->data[x + y * this->width] < last - 1) {
					this->data[x + y * this->width] = last - 1;
				}

				last = this->data[x + y * this->width];
			}
		}
		
		for (GGen_Coord y = this->height - 1; y > 0; y--) {
			GGen_Height last = this->data[y * this->width + this->width - 1];

			for (GGen_Coord x = this->width - 1; x > 0; x--) {
				if (mode == GGEN_ADDITIVE && this->data[x + y * this->width] > last + 1) {
					this->data[x + y * this->width] = last + 1;
				} else if (mode == GGEN_SUBSTRACTIVE && this->data[x + y * this->width] < last - 1) {
					this->data[x + y * this->width] = last - 1;
				}

				last = this->data[x + y * this->width];
			}
		}
	} else if (direction == GGEN_VERTICAL) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			GGen_Height last = this->data[x];
			
			for (GGen_Coord y = 0; y < this->height; y++) {
				if (mode == GGEN_ADDITIVE && this->data[x + y * this->width] > last + 1) {
					this->data[x + y * this->width] = last + 1;
				} else if (mode == GGEN_SUBSTRACTIVE && this->data[x + y * this->width] < last - 1) {
					this->data[x + y * this->width] = last - 1;
				}

				last = this->data[x + y * this->width];
			}
		}
		
		for (GGen_Coord x = this->width - 1; x > 0; x--) {
			GGen_Height last = this->data[this->length - (this->width - x)];

			for (GGen_Coord y = this->height - 1; y > 0; y--) {
				if (mode == GGEN_ADDITIVE && this->data[x + y * this->width] > last + 1) {
					this->data[x + y * this->width] = last + 1;
				} else if (mode == GGEN_SUBSTRACTIVE && this->data[x + y * this->width] < last - 1) {
					this->data[x + y * this->width] = last - 1;
				}

				last = this->data[x + y * this->width];
			}
		}
	}
}

void GGen_Data_2D::Normalize(GGen_Normalization_Mode mode) 
{
	this->NormalizeDirection(GGEN_HORIZONTAL, mode);
	this->NormalizeDirection(GGEN_VERTICAL, mode);
}

void GGen_Data_2D::Transform(double a11, double a12, double a21, double a22, bool preserve_size)
{
	/* The matrix must be invertible (its determinant must not be 0) */
	GGen_Script_Assert(a11 * a22 - a12 * a21 != 0);

	/* Calculate output's boundaries so we can allocate the new array */
	double new_top_right_x = (this->width - 1) * a11;
	double new_top_right_y = (this->width - 1) * a21;
	
	double new_bottom_left_x = (this->height - 1) * a12;
	double new_bottom_left_y = (this->height - 1) * a22;
	
	double new_bottom_right_x = (this->width - 1) * a11 + (this->height - 1) * a12;
	double new_bottom_right_y = (this->width - 1) * a21 + (this->height - 1) * a22;
	
	/* Find which bounding point is which (the rotations and such might change this). The zeros
	represent the origin (upper left corner), which always stays the same. */
	GGen_CoordOffset new_left_x = (int32) floor(MIN(MIN(0, new_top_right_x), MIN(new_bottom_left_x, new_bottom_right_x)));
	GGen_CoordOffset new_right_x = (int32) ceil(MAX(MAX(0, new_top_right_x), MAX(new_bottom_left_x, new_bottom_right_x)));
	
	GGen_CoordOffset new_top_y = (int32)floor(MIN(MIN(0, new_top_right_y), MIN(new_bottom_left_y, new_bottom_right_y)));
	GGen_CoordOffset new_bottom_y = (int32) ceil(MAX(MAX(0, new_top_right_y), MAX(new_bottom_left_y, new_bottom_right_y)));
	
	GGen_CoordOffset new_width = new_right_x - new_left_x + 1;
	GGen_CoordOffset new_height = new_bottom_y - new_top_y + 1;
	
	/* Make sure the output dimensions fit into a 16 bit unsigned integer, so we don't have array overflows later */
	GGen_Script_Assert(new_width < 2 << 16 && new_height < 2 << 16);
	
	/* New origin coordinates */
	int new_origin_x = -new_left_x;
	int new_origin_y = -new_top_y;
	
	/* Invert the transformation matrix */
	double inverted_a11 =   a22 / (-(a12 * a21) + a11 * a22);
	double inverted_a12 = -(a12 / (-(a12 * a21) + a11 * a22));
	double inverted_a21 = -(a21 / (-(a12 * a21) + a11 * a22));
	double inverted_a22 =   a11 / (-(a12 * a21) + a11 * a22);

	int from_x, to_x, from_y, to_y;
	GGen_Index new_length;
	GGen_Height* new_data;
	
	if (preserve_size) {
		/* Calculate boundaries of the centered box of the original size */
		from_x = ((signed) new_width - (signed) this->width) / 2;
		to_x = from_x + this->width;

		from_y = ((signed) new_height - (signed) this->height) / 2;
		to_y = from_y + this->height;
		
		new_length = this->length;
		new_width = this->width;
		new_height = this->height;
		
		/* Allocate the new array */
		new_data = new GGen_Height[this->length];
	} else { 
		/* Using the new image bounding box */
		from_x = from_y = 0;
		
		to_x = new_width;
		to_y = new_height;
		
		new_length = new_width * new_height;
		
		/* Allocate the new array */
		new_data = new GGen_Height[new_length];
	}
	
	/* Go through the new array and for every tile look back into the old array (thus we need the inverted function) what is there */
	for (GGen_CoordOffset new_y = from_y; new_y < to_y; new_y++) {
		/* The second multiplication always stays the same for whole row */
		GGen_Index y_part_1 = (GGen_Height) ((new_y - new_origin_y) * inverted_a12);
		GGen_Index y_part_2 = (GGen_Height) ((new_y - new_origin_y) * inverted_a22);
		
		/* Offset from pointer from the first cell in the array to the first cell in current row */
		GGen_CoordOffset y_offset = (new_y - from_y) * new_width;
		
		for (GGen_CoordOffset new_x = from_x; new_x < to_x; new_x++) {
			/* Calculate the original coordinates for the current "new point" by multiplying the coordinate vector of
			the desired point by inverted transformation matrix */
			GGen_CoordOffset x = (GGen_CoordOffset) ((new_x - new_origin_x) * inverted_a11) + y_part_1;
			GGen_CoordOffset y = (GGen_CoordOffset) ((new_x - new_origin_x) * inverted_a21) + y_part_2;
			
			/* The original point exists => use its value */
			if (x >= 0 && y >= 0 && x < this->width && y < this->height) {			
				new_data[new_x - from_x + y_offset] = this->data[x + y * this->width];
			} else {
				/* The "original point" is outside the array => such areas are filled with black */
				new_data[new_x - from_x + y_offset] = 0;
			}
		}
	}
	
	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;
	this->length = new_length;
	this->width = new_width;
	this->height = new_height;
}

void GGen_Data_2D::Rotate(int32 angle, bool preserve_size)
{
	/* Clamp the angle to the 0-360 range */
	angle = angle % 360;
	
	/* Degrees to radians */
	double angle_double = (double) angle * 3.14159 / 180;
	
	/* Build the rotation matrix */
	this->Transform(
		cos(angle_double), 
		sin(angle_double), 
		-sin(angle_double), 
		cos(angle_double), 
		preserve_size
	);
}

void GGen_Data_2D::Shear(int32 horizontal_shear, int32 vertical_shear, bool preserve_size){
	/* Vertical and horizontal shear == 1 ==> the transformation matrix would be non-invertible */
	GGen_Script_Assert(horizontal_shear != 1 || vertical_shear != 1);
	
	this->Transform(
		1, 
		vertical_shear, 
		horizontal_shear, 
		1, 
		preserve_size
	);
}
 
 void GGen_Data_2D::Flip(GGen_Direction direction){
	this->Transform(
		direction == GGEN_HORIZONTAL ? 1 : -1, 
		0,
		0,
		direction == GGEN_VERTICAL ? 1 : -1,
		false
	);
}

// helper data structure for the following algorithm
class GGen_FillPolygon_DownwardsEdge{
public:
	double x; /* X of the upper (and in process current) point */
	GGen_CoordOffset y; /* Y of the upper point */
	GGen_CoordOffset dy; /* Edge height */
	double dxy; /* Change of X while moving one pixel down */

	/* These two comparers will be needed to make use of the STL sort function */

	/* Comparer for sorting with criteria Y > X > DXY */
	static bool CompareWithY(GGen_FillPolygon_DownwardsEdge* a, GGen_FillPolygon_DownwardsEdge* b){
		/* Sort first by Y */
		if (a->y < b->y) return true;
		else if (b->y < a->y) return false;

		/* Then by X */
		if (a->x < b->x) return true;
		else if (b->x < a->x) return false;

		/* Then by X */
		if (a->dxy < b->dxy) return true;
		else return false;

		/* In the case the edges are completely identical order doesn't matter */
	}

	/* Comparer for sorting with criteria X > DXY */
	static bool CompareWithoutY(GGen_FillPolygon_DownwardsEdge* a, GGen_FillPolygon_DownwardsEdge* b){
		/* Then by X */
		if (a->x < b->x) return true;
		else if (b->x < a->x) return false;

		/* Then by X */
		if (a->dxy < b->dxy) return true;
		else return false;

		/* In the case the edges are completely identical order doesn't matter */
	}

	/* Returns true if the edge is ready to be removed from the current line edge list (the edge is ending on the current line) */
	static bool IsRemoveable(GGen_FillPolygon_DownwardsEdge* victim){
		return victim->dy == 0;
	}
};

void GGen_Data_2D::FillPolygon(GGen_Path* path, GGen_Height value){
	GGen_Script_Assert(path != NULL);

	/* Create a full list of all non-horizontal edges (this is line algorithm, we can skip horizontal edges) */
	list<GGen_FillPolygon_DownwardsEdge*> edges;
	for (GGen_Path::Iterator i = path->points.begin(); i != path->points.end();) {
		GGen_Point* currentPoint = &*i;
		GGen_Point* nextPoint = NULL;

		/* Move the iterator so we can access the next element */
		i++;

		/* Connect the ending point with the first point to create a closed loop */
		if (i != path->points.end()) {
			nextPoint = &*i;	
		} else {
			nextPoint = &*(path->points.begin());
		}

		/* Skip horizontal edges */
		if (currentPoint->GetY() == nextPoint->GetY()) {
			continue;
		}

		/* The edge being created */
		GGen_FillPolygon_DownwardsEdge* edge = new GGen_FillPolygon_DownwardsEdge();

		/* Swap the points in case the edge is pointing upwards (so it is always pointing downwards) */
		if (currentPoint->GetY() > nextPoint->GetY()) {
			GGen_Point* tempPoint = currentPoint;
			currentPoint = nextPoint;
			nextPoint = tempPoint;
		}

		/* Fill in the edge data */
		edge->x = currentPoint->GetX();
		edge->y = currentPoint->GetY();

		edge->dy =  nextPoint->GetY() - currentPoint->GetY();
		edge->dxy = (double) (nextPoint->GetX() - currentPoint->GetX()) / (double) edge->dy;

		edges.push_back(edge);
	}

	/* Sort the edges by given criteria */
	edges.sort(GGen_FillPolygon_DownwardsEdge::CompareWithY);

	/* Edges intersecting current line */
	list<GGen_FillPolygon_DownwardsEdge*> currentLineEdges;
	bool needsSorting = true;

	/* For each line from the top-most point to the bottom edge of the map */
	for (GGen_CoordOffset y = (*edges.begin())->y; y < this->height; y++) {

		/* Add edges starting on this line to the currently worked list */
		list<GGen_FillPolygon_DownwardsEdge*>::iterator i = edges.begin();
		while (i != edges.end() && (*i)->y == y) {
			currentLineEdges.push_back(*i);

			/* Lines in the working list are sorted only by X and DXY, so the working list will need resorting */
			needsSorting = true;

			i++;
		}

		/* If there are no edges in the current line, we can skip the remaining steps */
		if(currentLineEdges.size() == 0) continue;

		/* The edges can be added to the working list only once -> delete them from the main list */
		edges.erase(edges.begin(), i);
		
		/* Resort the list if requested */
		if (needsSorting) {
			currentLineEdges.sort(GGen_FillPolygon_DownwardsEdge::CompareWithoutY);

			needsSorting = false;
		}

		/* If we are in the map area, try to draw the pixels. */
		if(y >= 0){
			/* Fill every segment between even and odd edge intersection with the current line */
			bool odd = false;
			i = currentLineEdges.begin(); /* Reuse the iterator variable from above - it won't be needed anymore */
			for (GGen_Coord x = 0; x < this->width; x++) {
				/* There could be multiple intersections on the same pixel */
				while (i != currentLineEdges.end()) {
					if ((double) x > (*i)->x) {
						odd = !odd;
						i++;
					}
					else break;
				}

				/* If we are in the "odd section" (between even and odd intersection) */
				if (odd) {
					this->data[x + this->width * y] = value;
				}
			}
		}

		/* Update the current working line list for next line */
		double lastX = INT_MIN;
		for (list<GGen_FillPolygon_DownwardsEdge*>::iterator j = currentLineEdges.begin(); j != currentLineEdges.end(); j++) {
			GGen_FillPolygon_DownwardsEdge* edge = *j;

			/* Shorten the remaining piece of the edge by one */
			edge->dy--;

			/* Move X in direction of the edge */
			edge->x += edge->dxy;

			/* Detect disruptions in edge order. If there are any, request sort */
			if(lastX > edge->x){
				needsSorting= true;
			}

			lastX = edge->x;
		}

		/* Delete all edges that have DY == 0 (that are ending on current line) */
		currentLineEdges.remove_if(GGen_FillPolygon_DownwardsEdge::IsRemoveable);
	}	
}

/* Helper data structure for the following algorithm. */
struct GGen_StrokePath_Quad{
	GGen_Coord x, y;
	GGen_Size width, height;

	GGen_StrokePath_Quad(GGen_Coord x, GGen_Coord y, GGen_Size width, GGen_Size height):x(x), y(y), width(width), height(height){
	}
};


/* Helper function used to calculated distance from one map point to one path segment defined by two points. */
GGen_Height GGen_StrokePath_GetDistanceToSegment(GGen_Point segmentPoint1, GGen_Point segmentPoint2, GGen_Point mapPoint){
	GGen_Height currentDistance;
	
	double vx = segmentPoint1.x - mapPoint.x;
	double vy = segmentPoint1.y - mapPoint.y;
	double ux = segmentPoint2.x - segmentPoint1.x;
	double uy = segmentPoint2.y - segmentPoint1.y;

	double length = ux * ux + uy * uy;

	double det = (-vx * ux) + (-vy * uy);

	if(det < 0 || det > length)
	{
		ux = segmentPoint2.x - mapPoint.x;
		uy = segmentPoint2.y - mapPoint.y;
		currentDistance = (GGen_Distance) (sqrt(MIN(vx * vx + vy * vy, ux * ux + uy * uy)) + 0.5);						
	}
	else {
		det = ux * vy - uy * vx;
		currentDistance = (GGen_Distance) (sqrt((det * det) / length) + 0.5);
	}

	return currentDistance;
}

void GGen_Data_2D::StrokePath(GGen_Path* path, GGen_Data_1D* brush, GGen_Distance radius, bool fill_outside) {
	GGen_Script_Assert(path != NULL);
	GGen_Script_Assert(brush != NULL);
	GGen_Script_Assert(radius > 0);	

	/* This is quite typical divide-and-conquer algorithm. The map will be divided into four quads. The most adjacent
	 * path segment will be calculated for each of it corners. If the quad has all four corners adjacent to the same
	 * segment, no further subdivision will be needed and whole area will be painted with values depending on distances
	 * to the segment. If at least one corner of the quad is adjacent to a different segment, the quad will be divided
	 * into four more segments and the whole process will be repeated (not stopping until all the corners are adjacent
	 * to the same segment or quad size of 1x1 is reached). */

	/* Fast random access to path segments will be needed with this algorithm (the original std::list structure
	 * of course provides random access with linear complexity only = unacceptable). */
	vector<GGen_Point> segmentArray = vector<GGen_Point>(path->points.begin(), path->points.end());

	/* Distance from each tile to the nearest path segment. Tiles, that were not calculated yet hold GGEN_INVALID_HEIGHT. */
	GGen_Height* segmentDistances = new GGen_Height[this->length];

	/* Index of the nearest path segment (indexed from 0 from the beginning of the path). */
	uint32* segmentIndices = new uint32[this->length];

	/* Prefill working arrays with default data. */
	for(GGen_Index i = 0; i < this->length; i++){
		segmentDistances[i] = GGEN_INVALID_HEIGHT;
		segmentIndices[i] = 0;
	}
	
	/* This stack will be used to simulate recursion. True recursion is not advisable in this case,
	 * since recursion on huge maps (10000+) would lead to huge call stack depths, possibly triggering
	 * stack overflows. */
	stack<GGen_StrokePath_Quad> quadStack;

	/* First step will be on whole map. */
	quadStack.push(GGen_StrokePath_Quad(0, 0, this->width, this->height));

	/* Initiate the "recursion". */
	while(!quadStack.empty()){
		GGen_StrokePath_Quad currentQuad = quadStack.top();
		quadStack.pop();		

		/* Corner points in order: top-left, top-right, bottom-left, bottom-right. The points are arranged in
		 * an array, so and FOR cycle can work them easily. */
		GGen_Point cornerPoints[4] = 
			{
				GGen_Point(currentQuad.x, currentQuad.y), // top-left
				GGen_Point(currentQuad.x + currentQuad.width - 1, currentQuad.y), // top-right
				GGen_Point(currentQuad.x, currentQuad.y + currentQuad.height - 1), // bottom-left
				GGen_Point(currentQuad.x + currentQuad.width - 1, currentQuad.y + currentQuad.height - 1), // bottom-right
			};
		
		/* Segment index to which is the top-left corner the closest (this will be needed to tell if all the corners
		 * of the current quad are influenced by the same path segment). */
		uint32 topLeftCornerSegmentIndex = 0;
		
		bool isSubdivisionNeeded = false;

		/* For every corner of the current quad... */
		for(int cornerNumber = 0; cornerNumber < 4; cornerNumber++){
			GGen_Point currentPoint = cornerPoints[cornerNumber];
			GGen_Index currentIndex = currentPoint.x + this->width * currentPoint.y;
			
			if(segmentDistances[currentIndex] == GGEN_INVALID_HEIGHT){				
				/* Calculate distance to the closest path segment */
				GGen_Distance distance = this->GetMaxDistance();
				uint32 activeSegmentIndex = 0;
				uint32 currentSegmentIndex = 0;
				for(GGen_Path::Iterator i = path->points.begin(); i != path->points.end(); ){
					GGen_Point point1 = *i; 

					i++;

					if(i == path->points.end()) break;

					GGen_Point point2 = *i;

					GGen_Distance currentDistance = GGen_StrokePath_GetDistanceToSegment(point1, point2, currentPoint);

					if(currentDistance < distance){
						/* The segment is closer to the current tile than any segment tested yet. */
						distance = currentDistance;
						activeSegmentIndex = currentSegmentIndex;
					}
					else if(currentDistance == distance){
						/* The segment is tied with another segment, because at path segment joints there are TWO path
						 * segments at one coordinate. The tie will be resolved by comparing angles to the current point. 
						 * This will result in the area being split between the two segments "naturally", as if the two
						 * segments didn't meet at exactly the same spot, but were spaced from each other a little bit. */

						/* Calculate angles from the current path point to three different points. */

						/* Angle to the "previous" path segment (the segment defined by the previous point in the path). */
						double angleBackward = atan2((double) (point1.x - segmentArray[activeSegmentIndex].x), (double) (point1.y - segmentArray[activeSegmentIndex].y));
						
						/* Angle to the "next" path segment (the segment defined by the next point in the path). */
						double angleForward = atan2((double) (point1.x - point2.x), (double) (point1.y - point2.y));

						/* Angle to the current tile in the map. */
						double angleCurrent = atan2((double) (point1.x - currentPoint.x), (double) (point1.y - currentPoint.y));

						/* Calculate the angle defined by following points: previous path point, current path point, current map tile. */
						double angleDiffBackward = ABS(angleBackward - angleCurrent);
						
						/* Calculate the angle defined by following points: next path point, current path point, current map tile. */
						double angleDiffForward = ABS(angleForward - angleCurrent);

						/* We are calculating angle differences, it will aways be in range <0°, 180°> (angles greater than 
						 * 180° will be subtracted from 360°). */
						if(angleDiffBackward > 3.14){
							angleDiffBackward = 2 * 3.14 - angleDiffBackward;
						}

						if(angleDiffForward > 3.14){
							angleDiffForward = 2 * 3.14 - angleDiffForward;
						}

						/* The current path segment is by angle closer to the current map point than the other path segment. */
						if(angleDiffBackward > angleDiffForward){
							distance = currentDistance;
							activeSegmentIndex = currentSegmentIndex;						
						}
					}
					currentSegmentIndex++;
				}

				/* Write the calculated results into the arrays. */
				segmentDistances[currentIndex] = distance;
				segmentIndices[currentIndex] = activeSegmentIndex;
			}
			else{
				/* The distance has been already calculated */
			}
			
			if(cornerNumber == 0){
				/* This is the top-left corner - other corners will be compared with this one. */
				topLeftCornerSegmentIndex = segmentIndices[currentIndex];
			}
			else if(topLeftCornerSegmentIndex != segmentIndices[currentIndex]){
				/* Current corner's closest segment differs from the top-left corners' => further subdivision
				 * will be needed. */
				isSubdivisionNeeded = true;
			}
		}
				
		if(!isSubdivisionNeeded){
			/* Further subdivision is not needed, we know which segment to use to fill whole quad. */
			uint32 currentSegmentIndex = segmentIndices[currentQuad.x + this->width * currentQuad.y];
			GGen_Point point1 = segmentArray[currentSegmentIndex];
			GGen_Point point2 = segmentArray[currentSegmentIndex + 1];

			for(GGen_CoordOffset offsetY = 0; offsetY < currentQuad.height; offsetY++){
				for(GGen_CoordOffset offsetX = 0; offsetX < currentQuad.width; offsetX++){
					GGen_Coord x = currentQuad.x + offsetX;
					GGen_Coord y = currentQuad.y + offsetY;
					
					GGen_Distance currentDistance = GGen_StrokePath_GetDistanceToSegment(point1, point2, GGen_Point(x, y));

					segmentDistances[currentQuad.x + offsetX + this->width * (currentQuad.y + offsetY)] = currentDistance;
					segmentIndices[currentQuad.x + offsetX + this->width * (currentQuad.y + offsetY)] = currentSegmentIndex;
				}
			}
		}
		if(currentQuad.width > 1 || currentQuad.height > 1){
			/* Don't subdivide 1x1 quads. */

			/* Break the quad into four equal (almost equal - the dimensions might be even numbers) parts. */
			GGen_Size subdidedQuadWidth = currentQuad.width > 1 ? currentQuad.width / 2 : 1;
			GGen_Size subdidedQuadHeight = currentQuad.height > 1 ? currentQuad.height / 2 : 1;

			/* Subdivide horizontally and/or vertically (don't subdivide if only one tile in that direction is remaining). */
			quadStack.push(GGen_StrokePath_Quad(currentQuad.x, currentQuad.y, subdidedQuadWidth, subdidedQuadHeight));
			
			if(currentQuad.width > 1){
				quadStack.push(GGen_StrokePath_Quad(currentQuad.x + subdidedQuadWidth, currentQuad.y, currentQuad.width - subdidedQuadWidth, subdidedQuadHeight));
			}
			
			if(currentQuad.height > 1){
				quadStack.push(GGen_StrokePath_Quad(currentQuad.x, currentQuad.y + subdidedQuadHeight, subdidedQuadWidth, currentQuad.height - subdidedQuadHeight));
				if(currentQuad.width > 1){
					quadStack.push(GGen_StrokePath_Quad(currentQuad.x + subdidedQuadWidth, currentQuad.y + subdidedQuadHeight, currentQuad.width - subdidedQuadWidth, currentQuad.height - subdidedQuadHeight));
				}
			}
		}
	}

	/* Distance to all tiles have been calculated, map values into the main data array according to the brush. */ 
	for(GGen_Coord y = 0; y < this->height; y++){
		for(GGen_Coord x = 0; x < this->width; x++){
			GGen_Distance currentDistance = segmentDistances[x + this->width * y];
			
			if(currentDistance < radius){
				/* The distance is smaller than brush radius - get value form brush. */
				this->data[x + this->width * y] = brush->GetValueInterpolated(currentDistance, radius);
			}
			else if(fill_outside){
				/* The distance is larger than brush radius - use the outermost values from the brush (if asked to). */
				this->data[x + this->width * y] = brush->data[brush->length - 1];
			}
		}
	}

	delete [] segmentDistances;
	delete [] segmentIndices;
}

void GGen_Data_2D::FloodFillBase(GGen_Coord start_x, GGen_Coord start_y, GGen_Height fill_value, GGen_Comparison_Mode mode, GGen_Height threshold, bool select_only){
	GGen_Script_Assert(start_x < this->width);
	GGen_Script_Assert(start_y < this->height);		

	/* Bordering (potential spread) points will be stored in queue */
	queue<GGen_Point> queue;

	/* Add the starting point to the queue */
	queue.push(GGen_Point(start_x, start_y));

	/* Already processed points will be held in a simple bit mask to prevent entering infinite loop */
	vector<bool> mask(this->length, false);

	/* Keep filling until we have what to fill */
	while (!queue.empty()) {
		/* Pick one tile from top of the queue */
		GGen_Point current = queue.front();
		queue.pop();

		GGen_Height currentValue = this->data[current.x + current.y * this->width];

		/* Do not process one tile more than once */
		if(mask[current.x + current.y * this->width]) continue;

		/* Mark the current tile as processed */
		mask[current.x + current.y * this->width] = true;

		/* Check if the spread condition is valid for current tile */
		if (
			(mode == GGEN_EQUAL_TO && currentValue != threshold) ||
			(mode == GGEN_NOT_EQUAL_TO && currentValue == threshold) ||
			(mode == GGEN_LESS_THAN && currentValue >= threshold) ||
			(mode == GGEN_GREATER_THAN && currentValue <= threshold) ||
			(mode == GGEN_LESS_THAN_OR_EQUAL_TO && currentValue > threshold) ||
			(mode == GGEN_GREATER_THAN_OR_EQUAL_TO && currentValue < threshold))
		{
			/* The condition failed -> skip this tile */

			/* Fill the failed area with 0 in selection mode */
			if(select_only){			
				this->data[current.x + current.y * this->width] = 0;
			}

			continue;
		} 

		/* The condition is valid -> fill the current tile */
		this->data[current.x + current.y * this->width] = fill_value;

		/* Add surrounding tiles to the fill queue (as long as they are within mao borders and are yet unworked) */
		if (current.x > 0 && !mask[current.x - 1 + current.y * this->width]) {
			queue.push(GGen_Point(current.x - 1, current.y));
		}

		if (current.y > 0 && !mask[current.x + (current.y - 1) * this->width]) {
			queue.push(GGen_Point(current.x, current.y - 1));
		}

		if (current.x < this->width && !mask[current.x + 1 + current.y * this->width]) {
			queue.push(GGen_Point(current.x + 1, current.y));
		}
		
		if (current.y < this->height && !mask[current.x + (current.y + 1) * this->width]) {
			queue.push(GGen_Point(current.x, current.y + 1));
		}
	}

	/* Fill the unworked area with 0 in selection mode */
	if(select_only){			
		for (GGen_Coord y = 0; y < this->height; y++) {
			for (GGen_Coord x = 0; x < this->width; x++) {
				if(!mask[x + y * this->width]){
					this->data[x + y * this->width] = 0;
				}
			}	
		}
	}
}

void GGen_Data_2D::FloodFill(GGen_Coord start_x, GGen_Coord start_y, GGen_Height fill_value, GGen_Comparison_Mode mode, GGen_Height threshold){
	GGen_Script_Assert(start_x < this->width);
	GGen_Script_Assert(start_y < this->height);		

	this->FloodFillBase(start_x, start_y, fill_value, mode, threshold, false);
}

void GGen_Data_2D::FloodSelect(GGen_Coord start_x, GGen_Coord start_y, GGen_Comparison_Mode mode, GGen_Height threshold){
	GGen_Script_Assert(start_x < this->width);
	GGen_Script_Assert(start_y < this->height);		

	this->FloodFillBase(start_x, start_y, 1, mode, threshold, true);
}

GGen_Height GGen_Data_2D::GetValueOnPathBase(GGen_Path* path, bool max){
	GGen_Script_Assert(path != NULL);
	GGen_Script_Assert(path->points.size() > 1);

	GGen_Height extreme = max ? GGEN_MIN_HEIGHT : GGEN_MAX_HEIGHT;

	/* For every line segment do... */
	for (GGen_Path::Iterator i = path->points.begin(); i != path->points.end();) {
		GGen_Point* currentPoint = &*i;

		/* The last segment is between the last two points, end once we reach the last point */
		if(++i == path->points.end()){
			break;
		}

		GGen_Point* nextPoint = &*i;

		/* DDA line drawing algorithm (converted to line reading algorithm :) ) */

		/* The algorithm works only in <315°, 45°> range, we must use rotated variant for steeper segments */
		if(ABS(currentPoint->x - nextPoint->x) > ABS(currentPoint->y - nextPoint->y)){
			/* Swap the points in case the edge is pointing leftwards (so it is always pointing rightwards) */
			if (currentPoint->GetX() > nextPoint->GetX()) {
				//swap<GGen_Point*>(currentPoint, nextPoint);
				GGen_Point* tempPoint = currentPoint;
				currentPoint = nextPoint;
				nextPoint = tempPoint;
			}

			double y = currentPoint->y;
			double dy = currentPoint->x != nextPoint->x ? (double) (nextPoint->y - currentPoint->y) / (double) (nextPoint->x - currentPoint->x) : 0;

			GGen_CoordOffset x = currentPoint->x;	

			/* The first point */
			if(x > 0 && x < this->width && y > 0 && y < height){
				/* Y coordinate must be rounded */
				GGen_Height current = this->data[x + (GGen_CoordOffset) (y + 0.5) * this->width]; 

				if((max && extreme < current) || (!max && extreme > current)){
					extreme = current;
				} 
			}

			/* Rest of the line */
			while(x < nextPoint->x){
				x++;
				y += dy;

				if(x > 0 && x < this->width && (GGen_CoordOffset) y > 0 && (GGen_CoordOffset) y < height){
					/* Y coordinate must be rounded */
					GGen_Height current = this->data[x + (GGen_CoordOffset) (y + 0.5) * this->width]; 

					if((max && extreme < current) || (!max && extreme > current)){
						extreme = current;
					} 
				}
			}
		} else {
			/* Rotated variant */

			/* Swap the points in case the edge is pointing leftwards (so it is always pointing rightwards) */
			if (currentPoint->GetY() > nextPoint->GetY()) {
				//swap<GGen_Point*>(currentPoint, nextPoint);
				GGen_Point* tempPoint = currentPoint;
				currentPoint = nextPoint;
				nextPoint = tempPoint;
			}

			double x = currentPoint->x;
			double dx = currentPoint->y != nextPoint->y ? (double) (nextPoint->x - currentPoint->x) / (double) (nextPoint->y - currentPoint->y) : 0;

			GGen_CoordOffset y = currentPoint->y;	

			/* The first point */
			if(x > 0 && x < this->width && y > 0 && y < height){
				/* X coordinate must be rounded */
				GGen_Height current = this->data[(GGen_CoordOffset) (x + 0.5) + y * this->width]; 

				if((max && extreme < current) || (!max && extreme > current)){
					extreme = current;
				}
			}

			/* Rest of the line */
			while(y < nextPoint->y){
				y++;
				x += dx;

				if(x > 0 && x < this->width && (GGen_CoordOffset) y > 0 && (GGen_CoordOffset) y < height){
					/* X coordinate must be rounded */
					GGen_Height current = this->data[(GGen_CoordOffset) (x + 0.5) + y * this->width]; 

					if((max && extreme < current) || (!max && extreme > current)){
						extreme = current;
					}
				}
			}
		}

		
	}

	return extreme;
}

GGen_Height GGen_Data_2D::GetMaxValueOnPath(GGen_Path* path){
	GGen_Script_Assert(path != NULL);
	GGen_Script_Assert(path->points.size() > 1);

	return this->GetValueOnPathBase(path, true);
}

GGen_Height GGen_Data_2D::GetMinValueOnPath(GGen_Path* path){
	GGen_Script_Assert(path != NULL);
	GGen_Script_Assert(path->points.size() > 1);

	return this->GetValueOnPathBase(path, false);
}

void GGen_Data_2D::ExpandShrinkDirectionBase(GGen_Distance distance, GGen_Direction direction, bool shrink){
	GGen_Script_Assert(distance > 0);
	GGen_Script_Assert(direction == GGEN_VERTICAL || distance < this->width);
	GGen_Script_Assert(direction == GGEN_HORIZONTAL || distance < this->height);
	
	/* shrinking = inverse expanding */
	if(shrink){
		this->Invert();

		/* To fix maps that had non-negative values only */
		this->Add(1);
	}

	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[this->length];

	GGen_Script_Assert(new_data != NULL);

	/* The current window value is a count of values greater than 0 in the current window. */

	if (direction == GGEN_HORIZONTAL) {
		for (GGen_Coord y = 0; y < height; y++) {
			/* Prefill the window with value of the left edge + n leftmost values (where n is radius) */
			GGen_Size window_size = distance * 2 + 1;
			GGen_Height window_value = (GGen_Height) (this->data[this->width * y] > 0) * distance;

			for (GGen_Distance x = 0; x < distance; x++) {
				window_value += (GGen_Height) (this->data[x + this->width * y] > 0);
			}

			/* In every step shift the window one tile to the right  (= subtract its leftmost cell and add
			value of rightmost + 1). i represents position of the central cell of the window. */
			for (GGen_Coord x = 0; x < this->width; x++) {
				/* If the window is approaching right border, use the rightmost value as fill. */
				if (x < distance) {
					window_value += (GGen_Height) (this->data[x + distance + this->width * y] > 0) - (GGen_Height) (this->data[this->width * y] > 0);
				} else if (x + distance < this->width) {
					window_value += (GGen_Height) (this->data[x + distance + this->width * y] > 0) - (GGen_Height) (this->data[x - distance + this->width * y] > 0);
				} else {
					window_value += (GGen_Height) (this->data[this->width - 1 + this->width * y] > 0) - (GGen_Height) (this->data[x - distance + this->width * y] > 0);
				}

				/* Set the value of current tile to 1 as long as there is at least one value greater than zero in the current window. */
				new_data[x + this->width * y] = (GGen_Height) (window_value > 0) * (shrink ? -1 : 1);
			}
		}
	} else { /* vertical */
		for (GGen_Coord x = 0; x < this->width; x++) {
			/* Prefill the window with value of the left edge + n topmost values (where n is radius) */
			GGen_Size window_size = distance * 2 + 1;
			GGen_Height window_value = (GGen_Height) (this->data[x] > 0) * distance;

			for (GGen_Distance y = 0; y < distance; y++) {
				window_value += (GGen_Height) (this->data[x + y * this->width] > 0);
			}

			/* In every step shift the window one tile to the bottom  (= subtract its topmost cell and add
			value of bottommost + 1). i represents position of the central cell of the window. */
			for (GGen_Coord y = 0; y < this->height; y++) {
				/* If the window is approaching right border, use the rightmost value as fill. */
				if (y < distance) {
					window_value += (GGen_Height) (this->data[x + (y + distance) * this->width] > 0) - (GGen_Height) (this->data[x] > 0);
				} else if (y + distance < height) {
					window_value += (GGen_Height) (this->data[x + (y + distance) * this->width] > 0) - (GGen_Height) (this->data[x + (y - distance) * this->width] > 0);
				} else {
					window_value += (GGen_Height) (this->data[x + (this->height - 1) * this->width] > 0) - (GGen_Height) (this->data[x + (y - distance) * this->width] > 0);
				}

				/* Set the value of current tile to 1 as long as there is at least one value greater than zero in the current window. */
				new_data[x + this->width * y] = (GGen_Height) (window_value > 0) * (shrink ? -1 : 1);
			}
		}
	}	

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;	

	/* Shift the values from  range <-1, 0> to <0, 1> (we were working on an inverse) */
	if(shrink) this->Add(1);
}

void GGen_Data_2D::ExpandDirection(GGen_Distance distance, GGen_Direction direction){
	GGen_Script_Assert(distance > 0);
	GGen_Script_Assert(direction == GGEN_VERTICAL || distance < this->width);
	GGen_Script_Assert(direction == GGEN_HORIZONTAL || distance < this->height);

	this->ExpandShrinkDirectionBase(distance, direction, false);
}

void GGen_Data_2D::ShrinkDirection(GGen_Distance distance, GGen_Direction direction){
	GGen_Script_Assert(distance > 0);
	GGen_Script_Assert(direction == GGEN_VERTICAL || distance < this->width);
	GGen_Script_Assert(direction == GGEN_HORIZONTAL || distance < this->height);

	this->ExpandShrinkDirectionBase(distance, direction, true);
}

void GGen_Data_2D::Expand(GGen_Distance distance){
	GGen_Script_Assert(distance > 0);
	GGen_Script_Assert(distance < this->width);
	GGen_Script_Assert(distance < this->height);

	this->ExpandDirection(distance, GGEN_HORIZONTAL);
	this->ExpandDirection(distance, GGEN_VERTICAL);
}

void GGen_Data_2D::Shrink(GGen_Distance distance){
	GGen_Script_Assert(distance > 0);
	GGen_Script_Assert(distance < this->width);
	GGen_Script_Assert(distance < this->height);
	
	this->ShrinkDirection(distance, GGEN_HORIZONTAL);
	this->ShrinkDirection(distance, GGEN_VERTICAL);
}

void GGen_Data_2D::Outline(GGen_Comparison_Mode mode, GGen_Height threshold, GGen_Outline_Mode outlineMode)
{
	/* Outside border = inside border with inverted condition */
	if(outlineMode == GGEN_OUTSIDE){
		outlineMode = GGEN_INSIDE;

		switch(mode){
			case GGEN_EQUAL_TO: mode = GGEN_NOT_EQUAL_TO; break;
			case GGEN_NOT_EQUAL_TO: mode = GGEN_EQUAL_TO; break;
			case GGEN_GREATER_THAN: mode = GGEN_LESS_THAN_OR_EQUAL_TO; break;
			case GGEN_GREATER_THAN_OR_EQUAL_TO: mode = GGEN_LESS_THAN; break;
			case GGEN_LESS_THAN: mode = GGEN_GREATER_THAN_OR_EQUAL_TO; break;
			case GGEN_LESS_THAN_OR_EQUAL_TO:  mode = GGEN_GREATER_THAN; break;
		}
	}

	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[this->length];

	GGen_Script_Assert(new_data != NULL);
	
	/* First generate the border in horizontal direction... */
	for(GGen_Coord y = 0; y < this->height; y++){

		/* This value stays constant for all cells in the row. */
		GGen_Index yIndexOffset = y * this->width;

		/* A cell is part of a border, if it matches the condition and at least one of its neighbors doesn't. */
		for(GGen_Coord x = 1; x < this->width - 1; x++){
			GGen_Height prevValue = this->data[x - 1 + yIndexOffset];
			GGen_Height currentValue = this->data[x + yIndexOffset];
			GGen_Height nextValue = this->data[x + 1 + yIndexOffset];

			switch(mode){
				case GGEN_EQUAL_TO: currentValue = (GGen_Height) ((currentValue == threshold) && (!(nextValue == threshold) || !(prevValue == threshold))); break;
				case GGEN_NOT_EQUAL_TO: currentValue = (GGen_Height) ((currentValue != threshold) && (!(nextValue != threshold) || !(prevValue != threshold))); break;
				case GGEN_GREATER_THAN: currentValue = (GGen_Height) ((currentValue > threshold) && (!(nextValue > threshold) || !(prevValue > threshold))); break;
				case GGEN_GREATER_THAN_OR_EQUAL_TO: currentValue = (GGen_Height) ((currentValue >= threshold) && (!(nextValue >= threshold) || !(prevValue >= threshold))); break;
				case GGEN_LESS_THAN: currentValue = (GGen_Height) ((currentValue < threshold) && (!(nextValue < threshold) || !(prevValue < threshold))); break;
				case GGEN_LESS_THAN_OR_EQUAL_TO: currentValue = (GGen_Height) ((currentValue <= threshold) && (!(nextValue <= threshold) || !(prevValue <= threshold))); break;
			}

			new_data[x + yIndexOffset] = currentValue;
		}
	}

	/* First generate the border in vertical direction... */
	for(GGen_Coord x = 0; x < this->width; x++){

		/* A cell is part of a border, if it matches the condition and at least one of its neighbors doesn't. */
		for(GGen_Coord y = 1; y < this->height - 1; y++){
			GGen_Height prevValue = this->data[x + (y - 1) * this->width];
			GGen_Height currentValue = this->data[x + y * this->width];
			GGen_Height nextValue = this->data[x + (y + 1) * this->width];

			switch(mode){
				case GGEN_EQUAL_TO: currentValue = (GGen_Height) ((currentValue == threshold) && (!(nextValue == threshold) || !(prevValue == threshold))); break;
				case GGEN_NOT_EQUAL_TO: currentValue = (GGen_Height) ((currentValue != threshold) && (!(nextValue != threshold) || !(prevValue != threshold))); break;
				case GGEN_GREATER_THAN: currentValue = (GGen_Height) ((currentValue > threshold) && (!(nextValue > threshold) || !(prevValue > threshold))); break;
				case GGEN_GREATER_THAN_OR_EQUAL_TO: currentValue = (GGen_Height) ((currentValue >= threshold) && (!(nextValue >= threshold) || !(prevValue >= threshold))); break;
				case GGEN_LESS_THAN: currentValue = (GGen_Height) ((currentValue < threshold) && (!(nextValue < threshold) || !(prevValue < threshold))); break;
				case GGEN_LESS_THAN_OR_EQUAL_TO: currentValue = (GGen_Height) ((currentValue <= threshold) && (!(nextValue <= threshold) || !(prevValue <= threshold))); break;
			}

			/* The cell is in either "vertical border" or in "horizontal border" => it is part of the final border. */
			new_data[x + y * this->width] = MAX(currentValue, new_data[x + y * this->width]);
		}
	}
	

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;	
}

void GGen_Data_2D::ConvexityMap(GGen_Distance radius)
{
	GGen_Script_Assert(radius > 0);

	/* Convexity map is a difference between the current map and its smoothed variant. Smoothing erases any terrain features
	   that peak upwards (are convex) or bulge downwards (are concave). */ 
	GGen_Data_2D* unsmoothed = this->Clone();
	
	this->Smooth(radius);

	this->Invert();
	this->AddMap(unsmoothed);

	delete unsmoothed;
}

void GGen_Data_2D::Distort(GGen_Size waveLength, GGen_Distance amplitude)
{
	GGen_Script_Assert(waveLength > 0);
	GGen_Script_Assert(waveLength < this->width);
	GGen_Script_Assert(waveLength < this->height);
	GGen_Script_Assert(amplitude > 0);
	GGen_Script_Assert(amplitude < this->width);
	GGen_Script_Assert(amplitude < this->height);

	/* Set up an Amplitude object with one wave length only. */
	GGen_Amplitudes* amplitudeObject = new GGen_Amplitudes(waveLength);
	amplitudeObject->AddAmplitude(waveLength, amplitude);

	/* Use a different turbulence map for each coordinate (to prevent diagonal-oriented unnatural artifacts). */
	GGen_Data_2D* turbulenceXMap = new GGen_Data_2D(this->width, this->height, 0);
	turbulenceXMap->Noise(1, waveLength, amplitudeObject);

	GGen_Data_2D* turbulenceYMap = new GGen_Data_2D(this->width, this->height, 0);
	turbulenceYMap->Noise(1, waveLength, amplitudeObject);

	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[this->length];

	GGen_Script_Assert(new_data != NULL);

	/* Shift the coordinate of each cell by distortion values found in the turbulence maps. */
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			GGen_Coord distortedX = MAX(MIN((GGen_CoordOffset) x + (GGen_CoordOffset) turbulenceXMap->data[x + y * this->width], (GGen_CoordOffset) (this->width - 1)), 0);
			GGen_Coord distortedY = MAX(MIN((GGen_CoordOffset) y + (GGen_CoordOffset) turbulenceYMap->data[x + y * this->width], (GGen_CoordOffset) (this->height - 1)), 0);

			new_data[x + y * this->width] = this->data[distortedX + this->width * distortedY];
		}
	}

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;	

	delete amplitudeObject;
	delete turbulenceXMap;
	delete turbulenceYMap;
}

void GGen_Data_2D::NormalMap(){
	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[this->length];

	GGen_Script_Assert(new_data != NULL);

	/* Calculate facing direction information for individual cells */
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			new_data[x + y * this->width] = this->GetNormal(x, y);
		}	
	}

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;
}

void GGen_Data_2D::NormalDifferenceMap(int32 angle){
	/* Clamp the angle to the 0-360 range */
	angle = angle % 360;

	int32 oangle = angle;

	/* Rescale the angle to the <GGEN_MIN_HEIGHT, GGEN_MAX_HEIGHT> interval */
	angle = (int64) GGEN_MIN_HEIGHT + ((int64) angle * (int64) (GGEN_MAX_HEIGHT - GGEN_MIN_HEIGHT)) / (int64) 360;

	/* Allocate the new array */
	GGen_Height* new_data = new GGen_Height[this->length];

	GGen_Script_Assert(new_data != NULL);

	/* Calculate facing direction information for individual cells */
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {

			GGen_Height normal = this->GetNormal(x, y);

			/* Flat tiles aways return invalid angle */ 
			if(normal == GGEN_INVALID_HEIGHT) {
				new_data[x + y * this->width] = GGEN_INVALID_HEIGHT;
				continue;
			}

			/* Difference of the two angles */
			GGen_ExtHeight normalDifference = ABS(((GGen_ExtHeight) normal - (GGen_ExtHeight) angle));

			/* The push angles (180°, 360°) into the (0°, 180°) range (we want angle difference, not absolute angle) */
			if(normalDifference > (GGEN_MAX_HEIGHT - GGEN_MIN_HEIGHT) / 2){
				normalDifference = (GGEN_MAX_HEIGHT - GGEN_MIN_HEIGHT) - normalDifference;
			}

			new_data[x + y * this->width] = normalDifference;
		}	
	}

	/* Relink and delete the original array data */
	delete [] this->data;
	this->data = new_data;
}

GGen_Height GGen_Data_2D::GetNormal( GGen_Coord x, GGen_Coord y )
{
	GGen_Script_Assert(x < this->width);
	GGen_Script_Assert(y < this->height);

	GGen_Index indexLeft = x > 0 ? (x - 1) + y * this->width : x + y * this->width;
	GGen_Index indexRight = x < this->width ? (x + 1) + y * this->width : x + y * this->width;
	GGen_Index indexTop = y > 0 ? x + (y - 1) * this->width : x + y * this->width;
	GGen_Index indexBottom = y < this->height ? x + (y + 1) * this->width : x + y * this->width;

	GGen_Height heightLeft = this->data[indexLeft];
	GGen_Height heightRight = this->data[indexRight];
	GGen_Height heightTop = this->data[indexTop];
	GGen_Height heightBottom = this->data[indexBottom];

	double vectorAX = 2;
	double vectorAY = 0;
	double vectorAZ = (double) this->data[indexLeft] - (double) this->data[indexRight];

	double vectorBX = 0;
	double vectorBY = -2;
	double vectorBZ = (double) this->data[indexTop] - (double) this->data[indexBottom];

	/* The tile has upwards normal (all the surrounding tiles have the same height) */
	if(vectorAZ == 0 && vectorBZ == 0){
		return GGEN_INVALID_HEIGHT;
	}

	double productX = vectorAY * vectorBZ - vectorAZ * vectorBY;
	double productY = vectorAZ * vectorBX - vectorAX * vectorBZ;
	double productZ = vectorAX * vectorBY - vectorAY * vectorBX;

	double angle = atan2(productY, productX);

	return (GGen_Height) (angle / 3.14159 * GGEN_MAX_HEIGHT);
}

void GGen_Data_2D::SimpleErosion(uint8 numRounds, uint8 erosionFactor, bool enableSedimentation)
{
	//GGen_Script_Assert(this->width == flowMap->width && this->height == flowMap->height);
	//GGen_Script_Assert(this->width == sedimentMap->width && this->height == sedimentMap->height);

	// Each round is completely separate.
	for(uint32 round = 0; round < numRounds * this->length; round++){
		// Choose a random tile on the map.
		GGen_Coord x = GGen_Random<GGen_Coord>(0, this->width - 1);
		GGen_Coord y = GGen_Random<GGen_Coord>(0, this->height - 1);
		
		GGen_Height currentCarriedSediment = 0;

		// Don't bother with water tiles.
		if(this->data[x + this->width * y] <= 0) continue;

		// Keep advancing from the initial tile until the water level is reached.
		while(this->data[x + this->width * y] > 0){
			// Find the lowest neighboring tile
			GGen_Coord lowestNeighborX = x;
			GGen_Coord lowestNeighborY = y;
			GGen_Height lowestNeighborHeight = this->data[x + this->width * y];

			// Try to look at 5 random points in the neighborhood (to add a little randomness into the flow).
			for(uint8 i = 0; i < 5; i++){		
				GGen_CoordOffset currentNeighborX = (GGen_CoordOffset) x + GGen_Random<int8>(-1, 1);
				GGen_CoordOffset currentNeighborY = (GGen_CoordOffset) y + GGen_Random<int8>(-1, 1);

				// Ignore the neighbor if it is outside the map
				if(currentNeighborX < 0 || currentNeighborX == this->width || currentNeighborY < 0 || currentNeighborY == this->height){
					continue;
				}

				if(this->data[currentNeighborX + this->width * currentNeighborY] < lowestNeighborHeight){
					lowestNeighborX = currentNeighborX;
					lowestNeighborY = currentNeighborY;
					lowestNeighborHeight = this->data[currentNeighborX + this->width * currentNeighborY];
				}
			}

			// If no lower neighbor was found, try to lift the tile with the suspended sediment.
			if(lowestNeighborX == x && lowestNeighborY == y){
				// If we are out of sediment, terminate the flow.
				if(currentCarriedSediment <= 0) break;

				this->data[x + this->width * y] += erosionFactor;

				/*if(sedimentMap != null){
					sedimentMap->data[x + this->width * y]++;
				}*/

				// Consume the suspended sediment at twice the erosion rate (to prevent infinite cycles of erosion and sedimentation).
				currentCarriedSediment -= 2;
			}
			// A lower neighbor was found, lower the current tile and move the cursor to that neighbor.
			else {
				this->data[x + this->width * y] -= erosionFactor;
				
				/*if(sedimentMap != null){
					flowMap->data[x + this->width * y]++;
				}*/
				
				if(enableSedimentation){
					currentCarriedSediment += 1;
				}

				x = lowestNeighborX;
				y = lowestNeighborY;
			}
		}
	}
}

double GGen_Data_2D::FlowMap(double duration, double waterRate){
    GGen_Script_Assert(duration > 0);
    GGen_Script_Assert(waterRate > 0);

    GGen_ErosionSimulator simulator(this->width, this->height);
    double* heightMap = simulator.ImportHeightMap(*this);

    double* waterMap = new double[this->length];

    memset(waterMap, 0, this->length * sizeof(double));

    GGen_OutflowValues* outflowFluxMap = new GGen_OutflowValues[this->length];

    memset(outflowFluxMap, 0, this->length * sizeof(GGen_OutflowValues));

    for(double tRemaining = duration; tRemaining > 0; tRemaining -= simulator.deltaT){
        simulator.ApplyWaterSources(waterMap, waterRate);
        simulator.ApplyFlowSimulation(heightMap, waterMap, outflowFluxMap, NULL);
        simulator.ApplyEvaporation(waterMap);
    }

    for(GGen_Index i = 0; i < this->length; i++){
        if(this->data[i] <= 0){
            waterMap[i] = 0;
        }
    }

    double scale = simulator.ExportHeightMap(waterMap, *this);

    delete [] waterMap;
    delete [] heightMap;
    delete [] outflowFluxMap;

    return scale;
}
