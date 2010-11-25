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

/** 
 * @file ggen_path.h GGen_Path represents a continuous linear sequence of GGen_Point objects.
 **/

#pragma once

#include "ggen_support.h"
#include "ggen_point.h"
#include "ggen_data_2d.h"
#include <list>

/**
 * GGen_Path represents a continuous linear sequence of GGen_Point objects.
 **/
class GGen_ErosionSimulator{
	protected:

	public:
		GGen_ErosionSimulator();
		void Erode(GGen_Data_2D& heightMap);	
};