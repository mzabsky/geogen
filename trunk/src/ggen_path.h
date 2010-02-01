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
#include <list>

/**
 * GGen_Path represents a continuous linear sequence of GGen_Point objects.
 **/
class GGen_Path{
	public:
		typedef list<GGen_Point>::iterator Iterator;

		list<GGen_Point> points;
		bool closed;

		GGen_Path();

		void AddPoint(GGen_Point& point);
		void AddPointByCoords(GGen_CoordOffset x, GGen_CoordOffset y);
		void RemovePoint(uint32 index);
		void InsertPoint(uint32 index, GGen_Point& point);
		void InsertPointByCoords(uint32 index, GGen_CoordOffset x, GGen_CoordOffset y);
		void Clear();
		void Move(GGen_CoordOffset x, GGen_CoordOffset y);
		void Transform(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, double a11, double a12, double a21, double a22);
		void Rotate(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, int32 angle);
		void Shear(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, int32 horizontal_shear, int32 vertical_shear);
		void Flip(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, GGen_Direction direction);
		void Scale(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, double ratio_x, double ratio_y);
};