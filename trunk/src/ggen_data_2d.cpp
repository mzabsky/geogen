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

#include "ggen.h"
#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"
#include "ggen_path.h"
#include <assert.h>

uint16 GGen_Data_2D::num_instances = 0;
set<GGen_Data_2D*> GGen_Data_2D::instances;

GGen_Data_2D::GGen_Data_2D(GGen_Size width, GGen_Size height, GGen_Height value)
{
	GGen_Script_Assert(GGen::GetInstance()->GetStatus() != GGEN_LOADING_MAP_INFO);

	GGen_Script_Assert(width > 1 && height > 1);
	GGen_Script_Assert(width < GGen::GetMaxWidth() && height < GGen::GetMaxHeight());

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

GGen_Height GGen_Data_2D::GetValue(GGen_Coord x, GGen_Coord y)
{
	GGen_Script_Assert(x < this->width && y < this->height);
	
	return this->data[x + this->width * y];
}

GGen_Height GGen_Data_2D::GetValueInterpolated(GGen_Coord x, GGen_Coord y, GGen_Size scale_to_width, GGen_Size scale_to_height)
{
	GGen_Script_Assert(y < scale_to_height && x < scale_to_width);

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
	GGen_Script_Assert(new_width > 1 && new_height > 1);

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
	/* Walk through the items where the array and the addend with offset intersect */
	for (GGen_Coord y = MAX(0, offset_y); y < MIN(this->height, offset_y + addend->height); y++) {
		for (GGen_Coord x = MAX(0, offset_x); x < MIN(this->width, offset_x + addend->width); x++) {
			this->data[x + y * this->width] += addend->data[(x - offset_x) + (y - offset_y) * addend->width];
		}
	}
}

void GGen_Data_2D::AddMapMasked(GGen_Data_2D* addend, GGen_Data_2D* mask, bool relative)
{
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

void GGen_Data_2D::Union(GGen_Data_2D* victim)
{
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {	
			this->data[x + y * width] = MAX(this->data[x + y * this->width], victim->GetValueInterpolated(x, y, this->width, this->height));
		}
	}
}

void GGen_Data_2D::UnionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y)
{
	/* Walk through the items where the array and the victim with offset intersect */
	for (GGen_Coord y = MAX(0, offset_y); y < MIN(this->height, offset_y + victim->height); y++) {
		for (GGen_Coord x = MAX(0, offset_x); x < MIN(this->width, offset_x + victim->width); x++) {
			this->data[x + y * width] = MAX(victim->data[(x - offset_x) + (y - offset_y) * victim->width], this->data[x + y * this->width]);
		}
	}
}

void GGen_Data_2D::Intersection(GGen_Data_2D* victim)
{
	for(GGen_Coord y = 0; y < this->height; y++) {
		for(GGen_Coord x = 0; x < this->width; x++) {	
			this->data[x + y * this->width] = MIN(this->data[x + y * this->width], victim->GetValueInterpolated(x, y, this->width, this->height));
		}
	}
}

void GGen_Data_2D::IntersectionTo(GGen_Data_2D* victim, GGen_CoordOffset offset_x, GGen_CoordOffset offset_y)
{
	/* Walk through the items where the array and the addend with offset intersect */
	for (GGen_Coord y = MAX(0, offset_y); y < MIN(this->height, offset_y + victim->height); y++) {
		for (GGen_Coord x = MAX(0, offset_x); x < MIN(this->width, offset_x + victim->width); x++) {
			this->data[x + y * width] = MIN(victim->data[(x - offset_x) + (y - offset_y) * victim->width], this->data[x + y * this->width]);
		}
	}
}

void GGen_Data_2D::Combine(GGen_Data_2D* victim, GGen_Data_2D* mask, bool relative)
{
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
	GGen_ExtExtHeight target_x = to_x - from_x;
	GGen_ExtExtHeight target_y = to_y - from_y;
	
	/* Width of the gradient strip */
	double max_dist = sqrt((double) (ABS(to_x - from_x) * ABS(to_x - from_x) + ABS(to_y - from_y) * ABS(to_y - from_y)));

	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			GGen_ExtExtHeight point_x = x - from_x;
			GGen_ExtExtHeight point_y = y - from_y;

			/* Get the point on the gradient vector (vector goint through both starting and target point) to which is the current point closest */
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
	/* Call the profile gradient with linear profile */
	
	GGen_Data_1D temp(2, 0);
	temp.SetValue(0, from_value);
	temp.SetValue(1, to_value);

	this->GradientFromProfile(from_x, from_y, to_x, to_y, &temp, fill_outside);
}

void GGen_Data_2D::RadialGradientFromProfile(GGen_Coord center_x, GGen_Coord center_y, GGen_Distance radius, GGen_Data_1D* pattern, bool fill_outside)
{
	GGen_Script_Assert(radius > 0 && pattern != NULL);

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

void GGen_Data_2D::Noise(GGen_Size min_feature_size, GGen_Size max_feature_size, GGen_Amplitudes* amplitudes)
{
	GGen_Script_Assert(amplitudes != NULL);

	/* Prepare empty space for the work data */ 
	GGen_Height* new_data = new GGen_Height[this->length];

	GGen_Script_Assert(new_data != NULL);

	this->Fill(0);

	/* For each octave (goind from the higher wave lengths to the shorter)... */
	for (GGen_Size wave_length = max_feature_size; wave_length >= 1; wave_length /= 2) {
		GGen_Size frequency = GGen_log2(wave_length);
		GGen_Height amplitude = amplitudes->data[frequency];

		double pi_by_wave_length = 3.1415927 / wave_length;
		
		/* The wave length is shorter than the minimum desired wave length => done */
		if (wave_length < min_feature_size) break;

		/* Set up base noise grid values for  this round */
		for (GGen_Coord y = 0; y < this->height; y += wave_length) {
			for (GGen_Coord x = 0; x < this->width; x += wave_length) {
				new_data[x + y * this->width] = GGen_Random<GGen_Height>(-amplitude, amplitude);
			}
		}		

		if (wave_length > 1) {
			for (GGen_Coord y = 0; y < this->height; y++) {
				/* Precalculate some interpolation related values that are the same for whole */
				GGen_Coord vertical_remainder = y % wave_length;
				GGen_Coord nearest_vertical = y - vertical_remainder;
				double vertical_fraction = (1 - cos(vertical_remainder * pi_by_wave_length)) * .5;
			
				GGen_Index vertical_offset = nearest_vertical * this->width;
				GGen_Index vertical_offset_next = (nearest_vertical + wave_length) * this->width;
			
				for (GGen_Coord x = 0; x < this->width; x++) {
					/* We are on the grid ==> no need for the interpolation */
					if (vertical_remainder == 0 && x % wave_length == 0) continue;
	 
					/* Nearest horizontal noise grid coordinates */
					GGen_Coord nearest_horizontal = x - x % wave_length;

					/* Fetch values of four corners so we can interpolate the correct value. If such points don't 
					exist, wrap to the opposite border and pick a point from the opposite part of the array. This is
					not an attempt to create seamlessly repeatable noise (which has no point while creating terrains).
					The source points are picked just to have some data to interpolate with (to prevent creation of
					unpretty unnatural artifacts) */
					
					/* Upper left corner */
					GGen_Height upper_left = new_data[
						nearest_horizontal +
						vertical_offset
					];

					/* Upper right corner */
					GGen_Height upper_right;
					if (nearest_horizontal + wave_length > this->width - 1) {
						upper_right = new_data[vertical_offset];
					} else { 
						/* The X coord of the point overflows the right border of the map */
						upper_right = new_data[
							nearest_horizontal + wave_length +
							vertical_offset
						];	
					}
					
					/* Bottom left corner */
					GGen_Height bottom_left;
					if (nearest_vertical + wave_length > this->height - 1) {
						bottom_left = new_data[nearest_horizontal];
					} else {
						/* The Y coord of the point overflows the bottom border of the map */
						bottom_left = new_data[
							nearest_horizontal +
							vertical_offset_next
						];	
					}

					/* Bottom right corner */
					GGen_Height bottom_right;
					/* Both coords of the point overflow the borders of the map */
					if ((nearest_horizontal + wave_length > this->width - 1 && nearest_vertical + wave_length > this->height - 1) ) {
						bottom_right = new_data[0];
					} else if(nearest_horizontal + wave_length > this->width - 1) {
						/* The X coord of the point overflows the right border of the map */
						bottom_right = new_data[vertical_offset_next];
					} else if (nearest_vertical + wave_length > this->height - 1) {
						/* The Y coord of the point overflows the bottom border of the map */
						bottom_right = new_data[nearest_horizontal + wave_length];
					} else if( nearest_horizontal + wave_length + vertical_offset_next > (signed) (this->length - 1)) {
						/* Product of the coords owerflows the length of the array */
						bottom_right = new_data[0];
					} else {
						bottom_right = new_data[
							nearest_horizontal + wave_length +
							vertical_offset_next
						];	
					}

					/* Interpolate the value for the current tile from values of the four corners (using cosine interpolation) */
					double horizontal_fraction = (1 - cos( (x % wave_length) * pi_by_wave_length)) * .5;
					
					double interpolated_top = upper_left * (1 - horizontal_fraction) +  upper_right * horizontal_fraction;
					double interpolated_bottom = bottom_left * (1 - horizontal_fraction) + bottom_right * horizontal_fraction;

					data[x + y * this->width] +=(GGen_Height) (interpolated_top * ( 1 - vertical_fraction) + interpolated_bottom * vertical_fraction);
				}
			} 
		}

		/* Add the current octave to previous octaves */
		for(GGen_Coord y = 0; y < this->height; y += wave_length){
			for(GGen_Coord x = 0; x < this->width; x += wave_length){
				this->data[x + y * this->width] += new_data[x + y * this->width];
			}
		}
	}

	delete [] new_data;
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
		
			/* Collect points from the sorrounding cells */
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
	this->SmoothDirection(radius, GGEN_HORIZONTAL);
	this->SmoothDirection(radius, GGEN_VERTICAL);
}

void GGen_Data_2D::SmoothDirection(GGen_Distance radius, GGen_Direction direction)
{
	GGen_Script_Assert(radius > 0 && radius < this->width && radius < this->height);
	
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

			/* In every step shift the window one tile to the right  (= substract its leftmost cell and add
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

			/* In every step shift the window one tile to the bottom  (= substract its topmost cell and add
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

void GGen_Data_2D::Monochrome(GGen_Height treshold)
{
	for (GGen_Index i = 0; i < this->length; i++) {
		this->data[i] = this->data[i] > treshold ? 1 : 0;
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

	/* Fixt the corners */
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
	GGen_Height min = 0;
	GGen_Height max = GGEN_UNRELATIVE_CAP;

	if(relative){
		max = this->Max();
	}
	
	/* Smoothen the profile to prevent visible color jumps in the result */
	GGen_Data_1D profile_copy = *profile->Clone();
	profile_copy.ScaleTo(max + 1, false);
	if(profile_copy.length > 80) profile_copy.Smooth(max / 40);
	
	/* Make sure the smoothing didn't change the extremes */
	profile_copy.ScaleValuesTo(profile->Min(), profile->Max());
	
	/* Transform the values */
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {	
			if (this->data[x + y * this->width] > 0) {
				this->data[x + y * this->width] = profile_copy.GetValue(this->data[x + y * this->width]);
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
	
	/* Find which bounding point is which (the rotations and such might change this). The zeroes
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
	/* Verical and horizontal shear == 1 ==> the transformation matrix would be uninvertible */
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

	/* For each line ... */
	for (GGen_Coord y = 0; y < this->height; y++) {
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

		/* Fill every segment between even and odd egde intersection with the current line */
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

			/* If we are in the "odd setion" (between even and odd intersection) */
			if (odd) {
				this->data[x + this->width * y] = value;
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

			/* Detect distruptions in edge order. If there are any, request sort */
			if(lastX > edge->x){
				needsSorting= true;
			}

			lastX = edge->x;
		}

		/* Delete all edges that have DY == 0 (that are ending on current line) */
		currentLineEdges.remove_if(GGen_FillPolygon_DownwardsEdge::IsRemoveable);
	}	
}

void GGen_Data_2D::StrokePath(GGen_Path* path, GGen_Data_1D* brush, GGen_Distance radius, GGen_Height value) {
	for (GGen_Coord y = 0; y < this->height; y++) {
		for (GGen_Coord x = 0; x < this->width; x++) {
			
		}	
	}
}

void GGen_Data_2D::FloodFillBase(GGen_Coord start_x, GGen_Coord start_y, GGen_Height fill_value, GGen_Comparsion_Mode mode, GGen_Height treshold, bool select_only){
	GGen_Script_Assert(start_x < this->width && start_y < this->height);

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

		/* Do not proces one tile more than once */
		if(mask[current.x + current.y * this->width]) continue;

		/* Mark the current tile as processed */
		mask[current.x + current.y * this->width] = true;

		/* Check if the spread condition is valid for current tile */
		if (
			(mode == GGEN_EQUAL_TO && currentValue != treshold) ||
			(mode == GGEN_NOT_EQUAL_TO && currentValue == treshold) ||
			(mode == GGEN_LESS_THAN && currentValue >= treshold) ||
			(mode == GGEN_GREATER_THAN && currentValue <= treshold) ||
			(mode == GGEN_LESS_THAN_OR_EQUAL_TO && currentValue > treshold) ||
			(mode == GGEN_GREATER_THAN_OR_EQUAL_TO && currentValue < treshold))
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

		/* Add sorrounding tiles to the fill queue (as long as they are within mao borders and are yet unworked) */
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

void GGen_Data_2D::FloodFill(GGen_Coord start_x, GGen_Coord start_y, GGen_Height fill_value, GGen_Comparsion_Mode mode, GGen_Height treshold){
	GGen_Script_Assert(start_x < this->width && start_y < this->height);

	this->FloodFillBase(start_x, start_y, fill_value, mode, treshold, false);
}

void GGen_Data_2D::FloodSelect(GGen_Coord start_x, GGen_Coord start_y, GGen_Comparsion_Mode mode, GGen_Height treshold){
	GGen_Script_Assert(start_x < this->width && start_y < this->height);

	this->FloodFillBase(start_x, start_y, 1, mode, treshold, true);
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

	return this->GetValueOnPathBase(path, true);
}

GGen_Height GGen_Data_2D::GetMinValueOnPath(GGen_Path* path){
	GGen_Script_Assert(path != NULL);

	return this->GetValueOnPathBase(path, false);
}

void GGen_Data_2D::ExpandShrinkDirectionBase(GGen_Distance distance, GGen_Direction direction, bool shrink){
	GGen_Script_Assert(distance > 0 && distance < this->width && distance < this->height);
	
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
	this->ExpandShrinkDirectionBase(distance, direction, false);
}

void GGen_Data_2D::ShrinkDirection(GGen_Distance distance, GGen_Direction direction){
	this->ExpandShrinkDirectionBase(distance, direction, true);
}

void GGen_Data_2D::Expand(GGen_Distance distance){
	this->ExpandDirection(distance, GGEN_HORIZONTAL);
	this->ExpandDirection(distance, GGEN_VERTICAL);
}

void GGen_Data_2D::Shrink(GGen_Distance distance){
	this->ShrinkDirection(distance, GGEN_HORIZONTAL);
	this->ShrinkDirection(distance, GGEN_VERTICAL);
}