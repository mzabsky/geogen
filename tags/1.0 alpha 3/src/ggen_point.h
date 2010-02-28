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
 * @file ggen_point.h GGen_Point represents one point in GGen coordinate system.
 **/

#pragma once

#include "ggen_amplitudes.h"

/**
 * GGen_Point represents one point in GGen coordinate system.
 **/
class GGen_Point{
	public:
		GGen_CoordOffset x;
		GGen_CoordOffset y;

		GGen_Point(GGen_CoordOffset x, GGen_CoordOffset y);
		GGen_CoordOffset GetX();
		GGen_CoordOffset GetY();
		void SetX(GGen_CoordOffset x);
		void SetY(GGen_CoordOffset y);
		void SetCoords(GGen_CoordOffset x, GGen_CoordOffset y);
};