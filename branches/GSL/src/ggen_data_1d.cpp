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

#include <cstring>

#include "ggen_support.h"
#include "ggen_amplitudes.h"
#include "ggen_data_1d.h"
#include "ggen.h"

namespace GeoGen{
	Data_1D::Data_1D(Size length, Height value)
	{
		ScriptAssert(Generator::GetInstance()->GetStatus() != GGEN_LOADING_MAP_INFO);
		ScriptAssert(length <= Generator::GetMaxMapSize());
		ScriptAssert(length >= GGEN_MIN_MAP_SIZE);

		ScriptAssert(length > 1);

		this->length = length;

		/* Allocate the array */
		this->data = new Height[this->length];

		ScriptAssert(this->data != NULL);

		this->Fill(value);
	}

	Data_1D::~Data_1D()
	{
		delete [] this->data;
	}

	Data_1D* Data_1D::Clone()
	{
		Data_1D* victim = new Data_1D(this->length, 0);
	
		/* Copy the data */
		memcpy(victim->data, this->data, sizeof(Height) * victim->length);
	
		return victim;
	}

	Size Data_1D::GetLength()
	{
		return this->length;
	}
 
	Height Data_1D::GetValue(Coord x)
	{
		ScriptAssert(x < this->length);
	
		return this->data[x];
	}

	Height Data_1D::GetValueInterpolated(Coord x, Size scale_to_x)
	{
		ScriptAssert(scale_to_x >= GGEN_MIN_MAP_SIZE);
		ScriptAssert(x < scale_to_x);

		// No interpolation needed if the sizes are equal.
		if (scale_to_x == this->length) {
			return this->data[x];
		} else if (scale_to_x > this->length) {
			// The target scale is larger, interpolation is necessary.
			double ratio = (double) (scale_to_x - 1) / (double) (this->length - 1);

			// X coordinate in this array's coordinates.
			double scaledX = x / ratio;

			// How much does the source tile overlap over the smaller grid?
			double remainder = scaledX - (int) scaledX;

			// Heights to be interpolated.
			double value0 = scaledX > 1 ? data[(int) scaledX - 1] : (data[0] == 0 ? 1 : data[0]);
			double value1 = scaledX > 0 ? data[(int) scaledX] : data[0];
			double value2 = scaledX + 1 < this->length ? data[(int) scaledX + 1] : data[this->length - 1];
			double value3 = scaledX + 2 < this->length ? data[(int) scaledX + 2] : data[this->length - 1];

			// Coefficients for the cubic polynomial.
			double a0 = (value3 - value2) - (value0 - value1);
			double a1 = (value0 - value1) - a0;
			double a2 = value2 - value0;
			double a3 = value1;

			// Calculate value of the cubic polynomial.
			double result = a0 * remainder * remainder * remainder + a1 * remainder * remainder + a2 * remainder + a3;

			return (Height) result;
		}
		else {
			double ratio = (double) (scale_to_x - 1) / (double) (this->length - 1);

			// The target is smaller, pick the closest value.
			return (Height) data[(Coord) floor((double)x / ratio + 0.5)];
		}
	}

	void Data_1D::SetValue(Coord x, Height value)
	{
		ScriptAssert(x < this->length);

		this->data[x] = value;
	}

	void Data_1D::SetValueInRange(Coord from, Coord to, Height value)
	{
		ScriptAssert(from < this->length);
		ScriptAssert(to < this->length);
		ScriptAssert(from < to);	

		for (Coord i = from; i <= to; i++) {
			this->data[i] = value;
		}
	}

	void Data_1D::Fill(Height value)
	{
		for (Coord i = 0; i < this->length; i++) {
			this->data[i] = value;
		}
	}

	void Data_1D::Add(Height value)
	{
		for (Coord i = 0; i < this->length; i++) {
			this->data[i] += value;
		}
	}

	void Data_1D::AddArray(Data_1D* addend)
	{
		ScriptAssert(addend != NULL);

		/* Scale the addend as necessary */
		for (Coord i = 0; i < this->length; i++) {
			this->data[i] += addend->GetValueInterpolated(i, this->length);
		}
	}

	void Data_1D::AddTo(Data_1D* addend, CoordOffset offset)
	{
		ScriptAssert(addend != NULL);
		ScriptAssert(offset > -this->length);
		ScriptAssert(offset < this->length);

		/* Walk through the items where the array and the addend with offset intersect */
		for (Coord i = (Coord) MAX(0, offset); i < (Coord) MIN(this->length, offset + addend->length); i++) {
			this->data[i] += addend->data[i - offset];
		}
	}

	void Data_1D::AddArrayMasked(Data_1D* addend, Data_1D* mask, bool relative)
	{
		ScriptAssert(addend != NULL);
		ScriptAssert(mask != NULL);

		ExtHeight max = GGEN_UNRELATIVE_CAP;

		if(relative){
			max = mask->Max();
		}

		for(Coord i = 0; i < this->length; i++) {
			this->data[i] += (ExtHeight) addend->GetValueInterpolated(i, this->length) * (ExtHeight) mask->GetValueInterpolated(i, this->length) / max;
		}
	}

	void Data_1D::AddMasked(Height value, Data_1D* mask, bool relative)
	{	
		ScriptAssert(mask != NULL);

		Height max = GGEN_UNRELATIVE_CAP;
 
		if(relative){
			max = mask->Max();
		}

		for(Coord i = 0; i < this->length; i++) {
			this->data[i] += (ExtHeight) value * (ExtHeight) mask->GetValueInterpolated(i, this->length) / max;
		}
	}

	void Data_1D::Multiply(double value)
	{
		for (Coord i = 0; i < this->length; i++) {
			this->data[i] = (Height) ((double) this->data[i] * value);
		}
	}

	void Data_1D::MultiplyArray(Data_1D* factor)
	{
		ScriptAssert(factor != NULL);

		for (Coord i = 0; i < this->length; i++) {
			this->data[i] = (Height) ((double) this->data[i] * factor->GetValueInterpolated(i, this->length));
		}
	}

	void Data_1D::Invert()
	{
		for (Coord i = 0; i < this->length; i++) {
			this->data[i] = -this->data[i];
		}
	}

	void Data_1D::ScaleTo(Size new_length, bool scale_values)
	{
		ScriptAssert(new_length <= Generator::GetMaxMapSize());
		ScriptAssert(new_length >= GGEN_MIN_MAP_SIZE);

		ScriptAssert(new_length > 1);

		double ratio = new_length / this->length;

		/* Allocate the new array */
		Height* new_data = new Height[new_length];

		ScriptAssert(new_data != NULL);

		/* Fill the new array */
		for (Coord i = 0; i < new_length; i++) {
			new_data[i] = scale_values ? (Height) ((double) this->GetValueInterpolated(i, new_length) * ratio) : this->GetValueInterpolated(i, new_length);
		}

		/* Relink and delete the original array data */
		delete [] this->data;
		this->data = new_data;
		this->length = new_length;
	}

	void Data_1D::ScaleValuesTo(Height new_min, Height new_max)
	{
		ScriptAssert(new_max > new_min);

		Height min = this->Min();
		Height max = this->Max() - min;

		if (max == 0) {
			this->Fill(min);
			return;
		}

		new_max -= new_min;

		for (Coord i = 0; i < length; i++) {
			this->data[i] = new_min + (this->data[i] - min) * new_max / max;
		}
	}

	void Data_1D::Scale(double ratio, bool scale_values)
	{
		this->ScaleTo((Coord) ((double) this->length * ratio), scale_values);
	}

	void Data_1D::ResizeCanvas(Size new_length, CoordOffset new_zero)
	{
		ScriptAssert(new_length <= Generator::GetMaxMapSize());
		ScriptAssert(new_length >= GGEN_MIN_MAP_SIZE);
		ScriptAssert(new_zero <= Generator::GetMaxMapSize());
		ScriptAssert(new_zero >= -Generator::GetMaxMapSize());

		/* Allocate the new array */
		Height* new_data = new Height[new_length];

		ScriptAssert(new_data != NULL);

		for (Coord i = 0; i < new_length; i++) {
			if (i + new_zero >= 0 && i + new_zero < this->length){
				new_data[i] = this->data[i + new_zero];
			} else {
				new_data[i] = 0;
			}
		}

		/* Relink and delete the original array data */
		delete [] this->data;
		this->data = new_data;
		this->length = new_length;
	}

	void Data_1D::Clamp(Height min, Height max)
	{
		ScriptAssert(max > min);

		for (Coord i = 0; i < this->length; i++) {
			if(this->data[i] > max) this->data[i] = max;
			else if(this->data[i] < min) this->data[i] = min;
		}
	}

	void Data_1D::Flip()
	{
		Height temp;
	
		/* Go through the first half of the array and flip the value with its counterpart indexed from end */
		for (Coord i = 0; i < this->length / 2; i++) {
			temp = this->data[i];
			this->data[i] = this->data[this->length - 1 - i];
			this->data[length - 1 - i] = temp;
		}
	}

	Height Data_1D::Min()
	{
		Height temp = GGEN_MAX_HEIGHT;

		for (Coord i = 0; i < this->length; i++) {
			temp = temp > this->data[i] ? this->data[i] : temp;
		}

		return temp;
	}

	Height Data_1D::Max()
	{
		Height temp = GGEN_MIN_HEIGHT;

		for (Coord i = 0; i < this->length; i++){
			temp = temp < this->data[i] ? this->data[i] : temp;
		}

		return temp;
	}

	void Data_1D::Shift(CoordOffset distance, OverflowMode mode)
	{
		ScriptAssert(distance < this->length);
		ScriptAssert(distance > -this->length);

		ScriptAssert(distance < this->length && distance != 0 && distance > -this->length);
	
		/* Cycle mode */
		if (mode == GGEN_CYCLE) {
			/* Allocate the new array */
			Height* new_data = new Height[length];

			ScriptAssert(new_data != NULL);

			/* Fill the new array with shifted data */
			for (Coord i = 0; i < this->length; i++) {
				/* Some values can be just plainly shifted */
				if ((distance > 0 && i < this->length - distance) || (distance < 0 && (signed) i >= -distance)) {
					new_data[i + distance] = this->data[i];
				}
				/* Some must go through the right "border" */
				else if (distance > 0){
					new_data[i - this->length + distance] = this->data[i];
				}
				/* And some must go through the left "border" */
				else{
					new_data[i + this->length + distance] = this->data[i];
				}
			}

			/* Relink and delete the original data array */
			delete [] this->data;
			this->data = new_data;
		} else {
			/* Discard or Discard&fill mode */
			Height temp = 0;
		
			/* Positive distance -> shift right */
			if (distance > 0) {
				for (CoordOffset i = this->length - 1; i >= 0; i--) {
					/* Some values can be just plainly shifted... */
					if(i > distance - 1){
						this->data[i] = this->data[i - distance];
					
						if (mode == GGEN_DISCARD_AND_FILL) {
							temp = this->data[i];
						}
					} else {
						/* And some must be filled with zeros / closest value */
						if (mode == GGEN_DISCARD_AND_FILL) {
							this->data[i] = temp;
						} else {
							this->data[i] = 0;
						}
					}
				}	
			}
			/* Negative distance -> shift left */
			else{
				for (CoordOffset i = 0; i < length; i++) {
					/* Some values can be just plainly shifted... */
					if (i < this->length + distance) {
						this->data[i] = this->data[i - distance];
					
						if (mode == GGEN_DISCARD_AND_FILL) {
							temp = this->data[i];
						}
					} else {
						/* And some must be filled with zeros / closest value */
						if (mode == GGEN_DISCARD_AND_FILL){
							this->data[i] = temp;
						} else {
							this->data[i] = 0;
						}
					}
				}				
			}
		}
	}

	void Data_1D::Union(Data_1D* victim)
	{
		ScriptAssert(victim != NULL);

		for (Coord i = 0; i < this->length; i++) {
			this->data[i] = MIN(this->data[i], victim->GetValueInterpolated(i, this->length));
		}
	}

	void Data_1D::Intersection(Data_1D* victim)
	{
		ScriptAssert(victim != NULL);
	
		for (Coord i = 0; i < this->length; i++) {
			this->data[i] = MAX(this->data[i], victim->GetValueInterpolated(i, this->length));
		}
	}

	void Data_1D::Abs()
	{
		for (Index i = 0; i < this->length; i++) {
			if (this->data[i] < 0) {
				this->data[i] = -this->data[i];
			}
		}
	}

	void Data_1D::Monochrome(Height threshold)
	{
		for (Coord i = 0; i < this->length; i++) {
			this->data[i] = this->data[i] > threshold ? 1 : 0;
		}	
	}

	void Data_1D::SlopeMap()
	{
		/* Allocate the new array */
		Height* new_data = new Height[this->length];

		ScriptAssert(new_data != NULL);

		/* Calculate the slopes */
		for(Coord i = 1; i < this->length - 1; i++){
			new_data[i] = ABS(this->data[i - 1] - this->data[i + 1]);
		}

		/* Make sure the border items are treated correctly (use the neighboring values) */
		new_data[0] = new_data[1];
		new_data[this->length-1] = new_data[this->length-2];

		/* Relink and delete the original array data */
		delete [] this->data;
		this->data = new_data;	
	}

	void Data_1D::Normalize(NormalizationMode mode)
	{
		/* Additive mode */
		if (mode == GGEN_ADDITIVE) {
			/* Fix left-to-right "downhills" */
			for (Coord i = 1; i < this->length; i++) {
				if (this->data[i] < this->data[i - 1] - 1) {
					this->data[i] = this->data[i - 1] - 1;
				}
			}

			/* Fix right-to-left "downhills" */
			for (Coord i = this->length - 2; i > 0; i--) {
				if (this->data[i] < this->data[i + 1] - 1) {
					this->data[i] = this->data[i + 1] - 1;
				}
			}
		} else {
			/* Subtractive mode */
			/* Fix left-to-right "uphills" */
			for (Coord i = 1; i < this->length; i++) {
				if (this->data[i] > this->data[i - 1] + 1) {
					this->data[i] = this->data[i - 1] + 1;
				}
			}

			/* Fix right-to-left "uphills" */
			for (Coord i = this->length - 2; i > 0; i--) {
				if (this->data[i] > this->data[i + 1] + 1) {
					this->data[i] = this->data[i + 1] + 1;
				}
			}
		}

		/* Make sure the border items are treated correctly (use the neighboring values) */
		this->data[0] = this->data[1];
		this->data[this->length-1] = this->data[this->length-2];
	}

	void Data_1D::Gradient(Coord from, Coord to, Height from_value, Height to_value, bool fill_flat)
	{
		ScriptAssert(from < this->length);
		ScriptAssert(to < this->length);
	
		/* Swap values if necessary */
		if (from > to) {
			Coord temp = to;
			to = from;
			from = temp;
		
			Height temp2 = to_value;
			to_value = from_value;
			from_value = temp2;
		}

		Height base = from_value;
		ExtHeight offset = (ExtHeight) (to_value - base);
		Distance max_distance = to - from;
	
		for (Coord i = 0; i < this->length; i++) {
			/* Calculate current distance from "from" and "to" */
			Distance distance_from = (Distance) ABS(i - from);
			Distance distance_to = (Distance) ABS(i - to);

			if (distance_from > max_distance) {
				this->data[i] = fill_flat ? to_value: this->data[i];
			} else if (distance_to > max_distance) {
				this->data[i] = fill_flat ? from_value : this->data[i];
			} else {
				this->data[i] = base + (Height) (offset * (signed) distance_to / (signed) max_distance);
			}
		}
	}

	void Data_1D::Noise(Size min_feature_size, Size max_feature_size, Amplitudes* amplitudes)
	{
		ScriptAssert(amplitudes != NULL);
		ScriptAssert(min_feature_size >= 1);
		ScriptAssert(min_feature_size <= Generator::GetMaxMapSize());
		ScriptAssert(max_feature_size >= 1);
		ScriptAssert(max_feature_size <= Generator::GetMaxMapSize());

		uint16 frequency = Log2(max_feature_size);
		Height amplitude = amplitudes->data[frequency];

		Height* new_data = new Height[this->length];

		ScriptAssert(new_data != NULL);

		this->Fill(0);

		for (Size wave_length = max_feature_size; wave_length >= 1; wave_length /= 2) {
			frequency = Log2(wave_length);
			amplitude = amplitudes->data[frequency];

			if (wave_length < min_feature_size) break;

			for (Coord i = 0; i < this->length; i += wave_length) {
				new_data[i] = Random<Height>(-amplitude, amplitude);
			}

			if (wave_length > 1) {
				for (Coord i = 0; i < this->length; i++) {
					if (i % wave_length == 0) continue;

					Coord nearest = i - i % wave_length;

					Coord next = i - i % wave_length + wave_length;

					next = next >= this->length ? 0 : next;

					double vertical = (1 - cos( (i % wave_length) * 3.1415927 / wave_length)) * .5;

					this->data[i] +=  (Height) ( (double) new_data[nearest] * (1 - vertical) + (double) new_data[next] * vertical);
				} 
			}

			for (Coord i = 0; i < this->length; i += wave_length) {
				this->data[i] += new_data[i];
			}

		}

		delete [] new_data;
	}

	void Data_1D::Smooth(Distance radius)
	{
		ScriptAssert(radius >= 1);
		ScriptAssert(radius <= this->length);
	
		/* Allocate the new array */
		Height* new_data = new Height[this->length];

		ScriptAssert(new_data != NULL);

		/* Calculate size of the filter window */
		Size window_size = radius * 2 + 1;

		/* Prefill the window with value of the left edge + n leftmost values (where n is radius) */
		ExtHeight window_value = this->data[0] * radius;

		for (Distance i = 0; i < radius; i++) {
			window_value += this->data[i];
		}

		/* In every step shift the window one tile to the right  (= subtract its leftmost cell and add
		value of rightmost + 1). i represents position of the central cell of the window. */
		for(Coord i = 0; i < this->length; i++){
			/* If the window is approaching right border, use the rightmost value as fill. */
			if(i < radius){
				window_value += this->data[i + radius] - this->data[0];
			}
			else if(i + radius < this->length){
				window_value += this->data[i + radius] - this->data[i - radius];
			}
			else{
				window_value += this->data[this->length - 1] - this->data[i - radius];
			}

			/* Set the value of current tile to arithmetic average of window tiles. */
			new_data[i] = window_value / window_size;
		}

		/* Relink and delete the original array data */
		delete [] this->data;
		this->data = new_data;
	}

	void Data_1D::Flood(double water_amount)
	{
		ScriptAssert(water_amount < 1);
		ScriptAssert(water_amount > 0);

		/* Calculate how many tiles should be flooded in total */
		Index target = (Index) (water_amount * (double) this->length);
	
		Index last_amount = 0;

		Height level = this->Min();
		Height max = this->Max();

		/* Go through the array values from bottom up and try to find the best fit to target water amount */
		while (level < max) {
			/* Calculate the amount of waters above current level */
			Index amount = 0;
			for (Coord i = 0; i < this->length; i++) {
				if (this->data[i] >= level) {
					amount++;
				}
			}

			/* Is current level higher than the target? */
			if (amount <= target) {
				/* Find if this level fits better than the previous (the closest fit applies) */
				if (amount - target < target - last_amount) {
					break;
				} else {
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

	Path* Data_1D::ToPath(uint16 point_count){
		ScriptAssert(point_count <= GGEN_MAX_PATH_LENGTH);
	
		Path* path = new Path;
	
		for(Index i = 0; i < point_count; i++){
			path->AddPointByCoords(i, this->GetValueInterpolated(i, point_count));
		}

		return path;
	}
}