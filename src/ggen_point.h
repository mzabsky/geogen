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

		/**
		 * Creates a new point object.
		 * @param x X coordinate of the point.
		 * @param y Y coordinate of the point.
		 **/
		GGen_Point(GGen_CoordOffset x, GGen_CoordOffset y);

		/**
		 * Returns X coordinate of the point.
		 * @return X coordinate of the point.
		 **/
		GGen_CoordOffset GetX();
		
		/**
		 * Returns Y coordinate of the point.
		 * @return Y coordinate of the point.
		 **/
		GGen_CoordOffset GetY();

		/**
		 * Sets X coordinate of the point.
		 * @param x X coordinate of the point.
		 **/
		void SetX(GGen_CoordOffset x);
		
		/**
		 * Sets Y coordinate of the point.
		 * @param y Y coordinate of the point.
		 **/
		void SetY(GGen_CoordOffset y);

		/**
		 * Sets coordinates of the point.
		 * @param x X coordinate of the point.
		 * @param y Y coordinate of the point.
		 **/
		void SetCoords(GGen_CoordOffset x, GGen_CoordOffset y);
};