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

		/**
		 * Creates a new empty Path object.
		 **/
		GGen_Path();

		/**
		 * Adds a point to the end of the path.
		 * @param point Point to be added.
		 **/
		void AddPoint(GGen_Point& point);
		
		/**
		 * Adds a point to the end of the path.
		 * @param x X coordinate of the point to be added.
		 * @param y Y coordinate of the point to be added.
		 **/
		void AddPointByCoords(GGen_CoordOffset x, GGen_CoordOffset y);

		/**
		 * Removes a single point from the path.
		 * @param index Index of the point to be removed (starting at 0).
		 **/
		void RemovePoint(uint32 index);

		/**
		 * Inserts a point into specific place in the path.
		 * @param index Index of the point, after which the new point is inserted (starting at 0).
		 * @param point Point to be added.
		 **/
		void InsertPoint(uint32 index, GGen_Point& point);
		
		/**
		 * Inserts a point into specific place in the path.
		 * @param index Index of the point, after which the new point is inserted (starting at 0).
		 * @param x X coordinate of the point to be added.
		 * @param y Y coordinate of the point to be added.
		 **/
		void InsertPointByCoords(uint32 index, GGen_CoordOffset x, GGen_CoordOffset y);

		/** 
		 * Returns X coordinate of the leftmost point in the path.
		 * @return X coordinate of the leftmost point in the path.
		 **/
		GGen_CoordOffset GetMinX();
		
		/** 
		 * Returns Y coordinate of the topmost point in the path.
		 * @return Y coordinate of the topmost point in the path.
		 **/
		GGen_CoordOffset GetMinY();

		/**
		 * Returns X coordinate of the rightmost point in the path.
		 * @return X coordinate of the rightmost point in the path.
		 **/
		GGen_CoordOffset GetMaxX();

		/**
		 * Returns Y coordinate of the bottommost point in the path.
		 * @return Y coordinate of the bottommost point in the path.
		 **/
		GGen_CoordOffset GetMaxY();

		/**
		 * Returns X coordinate of path's center of mass.
		 * @return X coordinate of path's center of mass.
		 **/
		GGen_CoordOffset GetAverageX();
		
		/** 
		 * Returns Y coordinate of path's center of mass.
		 * @return Y coordinate of path's center of mass.
		 **/
		GGen_CoordOffset GetAverageY();

		/**
		 * Returns X coordinate of path's unweighted center.
		 * @return X coordinate of path's unweighted center.
		 **/
		GGen_CoordOffset GetCenterX();
		
		/** 
		 * Returns X coordinate of path's unweighted center.
		 * @return X coordinate of path's unweighted center.
		 **/
		GGen_CoordOffset GetCenterY();

		/**
		 * Removes all points from the path.
		 **/
		void Clear();

		/**
		 * Moves all points in the path by an offset.
		 * @param x X offset.
		 * @param y Y offset.
		 **/
		void Move(GGen_CoordOffset x, GGen_CoordOffset y);

		/**
		 * Applies a linear transformation matrix onto the path.
		 * @param origin_x X coordinate of the transformation origin.
		 * @param origin_y Y coordinate of the transformation origin.
		 * @param a11 Matrix element (1,1).
		 * @param a12 Matrix element (1,2).
		 * @param a21 Matrix element (2,1).
		 * @param a22 Matrix element (2,2).
		 * @note Unlike with GGen_Data_2D, this operation is lossless (unless the matrix is degenerate).
		 **/
		void Transform(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, double a11, double a12, double a21, double a22);
		
		/**
		 * Rotates whole path around a point.
		 * @param origin_x X coordinate of the point around which is the rotation performed.
		 * @param origin_y Y coordinate of the point around which is the rotation performed.
		 * @param angle Rotation angle in degrees.
		 **/
		void Rotate(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, int32 angle);

		/**
		 * Shears the path vertically and/or horizontally.
		 * @param origin_x X coordinate of shear origin.
		 * @param origin_y Y coordinate of shear origin.
		 * @param horizontal_shear Horizontal shear factor (0 means no horizontal shearing).
		 * @param vertical_shear Horizontal shear factor (1 means no horizontal shearing).
		 **/
		void Shear(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, int32 horizontal_shear, int32 vertical_shear);

		/**
		 * Flips the path vertically or horizontally.
		 * @param origin_x X coordinate of flip origin.
		 * @param origin_y Y coordinate of flip origin.
		 * @param direction Direction along which is the flip performed.
		 **/
		void Flip(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, GGen_Direction direction);

		/**
		 * Scales the path.
		 * @param origin_x X coordinate of scale origin.
		 * @param origin_y Y coordinate of scale origin.
		 * @param ratio_x Horizontal scale ratio.
		 * @param ratio_y Vertical scale ratio
		 **/
		void Scale(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, double ratio_x, double ratio_y);
};