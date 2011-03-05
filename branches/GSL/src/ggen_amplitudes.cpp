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

namespace GeoGen {
	Amplitudes::Amplitudes(Size max_feature_size)
	{ 
		ScriptAssert(Generator::GetInstance()->GetStatus() != GGEN_LOADING_MAP_INFO);

		Size size = Log2(max_feature_size) + 1;

		this->data = new uint16[size];
		this->length = size;

		ScriptAssert(this->data != NULL);

		for (Index i = 0; i < size; i++) {
			this->data[i] = 0;
		}
	};

	void Amplitudes::AddAmplitude(uint16 feature_size, uint16 amplitude)
	{
		ScriptAssert(Log2(feature_size) < this->length);

		this->data[Log2(feature_size)] = amplitude;
	}
}