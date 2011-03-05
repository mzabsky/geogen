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
#pragma once

#include "ggen_support.h"

/** 
 * @file ggen_amplitudes.h GGen_Amplitudes is an object storing amplitude settings for GGen_Data_1D::Noise and GGen_Data_2D::Noise.
 **/

/**
 * GGen_Amplitudes is an object storing amplitude settings for GGen_Data_1D::Noise and GGen_Data_2D::Noise.
 **/
class GGen_Amplitudes{
	public:
		uint16* data;
		GGen_Size length;

		/**
		 * Creates a new GGen_Amplitudes object.
		 * @param max_feature_size The object will have space for amplitudes for wave lengths up to this number.
		 **/
		GGen_Amplitudes(GGen_Size max_feature_size);

		~GGen_Amplitudes(){ 
			delete [] data; 
		}

		/**
		 * Adds a new amplitude to the object.
		 * @param feature_size Wave length this amplitude will be assigned to.
		 * @param amplitude The amplitude value.
		 **/
		void AddAmplitude(uint16 feature_size, uint16 amplitude);
};
