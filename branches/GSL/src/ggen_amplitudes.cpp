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

#include <iostream>

#include "ggen.h"
#include "ggen_squirrel.h"
#include "ggen_support.h"
#include "ggen_amplitudes.h"

GGen_Amplitudes::GGen_Amplitudes(GGen_Size max_feature_size)
{ 
	GGen_Script_Assert(GGen::GetInstance()->GetStatus() != GGEN_LOADING_MAP_INFO);

	GGen_Size size = GGen_log2(max_feature_size) + 1;

	this->data = new uint16[size];
	this->length = size;

	GGen_Script_Assert(this->data != NULL);

	for (GGen_Index i = 0; i < size; i++) {
		this->data[i] = 0;
	}
};

void GGen_Amplitudes::AddAmplitude(uint16 feature_size, uint16 amplitude)
{
	GGen_Script_Assert(GGen_log2(feature_size) < this->length);

	this->data[GGen_log2(feature_size)] = amplitude;
}