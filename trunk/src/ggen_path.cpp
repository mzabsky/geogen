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

#include "ggen_support.h"
#include "ggen.h"
#include "ggen_path.h"

GGen_Path::GGen_Path() {
	GGen_Script_Assert(GGen::GetInstance()->GetStatus() != GGEN_LOADING_MAP_INFO);
}

void GGen_Path::AddPoint(GGen_Point& point){
	GGen_Script_Assert(this->points.size() + 1 < GGEN_MAX_PATH_LENGTH);

	this->points.push_back(point);
}

void GGen_Path::AddPointByCoords(GGen_CoordOffset x, GGen_CoordOffset y){
	GGen_Script_Assert(this->points.size() + 1 < GGEN_MAX_PATH_LENGTH);
	
	this->points.push_back(GGen_Point(x, y));
}

void GGen_Path::RemovePoint(uint32 index){
	GGen_Script_Assert(index < this->points.size());
	
	Iterator it = points.begin();
	for(unsigned i = 0; i < index; i++){
		it++;
	}

	this->points.erase(it);
}

void GGen_Path::InsertPoint(uint32 index, GGen_Point& point){
	GGen_Script_Assert(this->points.size() + 1 < GGEN_MAX_PATH_LENGTH);
	GGen_Script_Assert(index < this->points.size());
	
	Iterator it = points.begin();
	for(unsigned i = 0; i < index; i++){
		it++;
	}		

	points.insert(it, point);
}

void GGen_Path::InsertPointByCoords(uint32 index, GGen_CoordOffset x, GGen_CoordOffset y){
	GGen_Script_Assert(this->points.size() + 1 < GGEN_MAX_PATH_LENGTH);
	GGen_Script_Assert(index < this->points.size());
	
	GGen_Point temp(x, y);

	this->InsertPoint(index, temp);
}

void GGen_Path::Clear(){
	this->points.clear();
}

void GGen_Path::Move(GGen_CoordOffset x, GGen_CoordOffset y){
	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		(*i).x += x;
		(*i).y += y;
	}
}

void GGen_Path::Transform(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, double a11, double a12, double a21, double a22){
	/* Multiply the coordinate vector of each point by the transformation matrix */
	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		GGen_CoordOffset x = (*i).x;
		(*i).x = origin_x + (GGen_CoordOffset) ((double) (x - origin_x) * a11 + (double) ((*i).y - origin_y) * a12);
		(*i).y = origin_y + (GGen_CoordOffset) ((double) (x - origin_x) * a21 + (double) ((*i).y - origin_y) * a22);
	}	
}

void GGen_Path::Rotate(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, int32 angle)
{
	/* Clamp the angle to the 0-360 range */
	angle = angle % 360;
	
	/* Degrees to radians */
	double angle_double = (double) angle * 3.14159 / 180;
	
	/* Build the rotation matrix */
	this->Transform(origin_x, origin_y,
		cos(angle_double), 
		sin(angle_double), 
		-sin(angle_double), 
		cos(angle_double)
	);
}

void GGen_Path::Shear(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, int32 horizontal_shear, int32 vertical_shear){
	this->Transform(origin_x, origin_y,
		1, 
		vertical_shear, 
		horizontal_shear, 
		1
	);
}
 
 void GGen_Path::Flip(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, GGen_Direction direction){
	this->Transform(origin_x, origin_y,
		direction == GGEN_HORIZONTAL ? 1 : -1, 
		0,
		0,
		direction == GGEN_VERTICAL ? 1 : -1
	);
}

 void GGen_Path::Scale(GGen_CoordOffset origin_x, GGen_CoordOffset origin_y, double ratio_x, double ratio_y){
	 GGen_Script_Assert(ratio_x > 0 && ratio_y > 0);
	 
	 this->Transform(origin_x, origin_y,
		ratio_x, 
		0,
		0,
		ratio_y
	);
}

GGen_CoordOffset GGen_Path::GetMinX(){
	GGen_CoordOffset min = 30000;

	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		if((*i).x < min) min = (*i).x;
	}

	return min;
}

GGen_CoordOffset GGen_Path::GetMinY(){
	GGen_CoordOffset min = 30000;

	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		if((*i).y < min) min = (*i).y;
	}

	return min;
}

GGen_CoordOffset GGen_Path::GetMaxX(){
	GGen_CoordOffset max = -30000;

	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		if((*i).x > max) max = (*i).x;
	}

	return max;
}

GGen_CoordOffset GGen_Path::GetMaxY(){
	GGen_CoordOffset max = -30000;

	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		if((*i).y > max) max = (*i).y;
	}

	return max;
}

GGen_CoordOffset GGen_Path::GetCenterY(){
	GGen_CoordOffset max = -30000;
	GGen_CoordOffset min = 30000;

	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		if((*i).y > max) max = (*i).y;
		if((*i).y < min) min = (*i).y;
	}

	return (max - min) / 2;
}

GGen_CoordOffset GGen_Path::GetCenterX(){
	GGen_CoordOffset max = -30000;
	GGen_CoordOffset min = 30000;

	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		if((*i).x > max) max = (*i).x;
		if((*i).x < min) min = (*i).x;
	}

	return (max - min) / 2;
}

GGen_CoordOffset GGen_Path::GetAverageX(){
	int64 sum = 0;

	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		sum += (*i).x;
	}

	return (GGen_CoordOffset) (sum / this->points.size());
}

GGen_CoordOffset GGen_Path::GetAverageY(){
	int64 sum = 0;

	for(GGen_Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
		sum += (*i).y;
	}

	return (GGen_CoordOffset) (sum / this->points.size());
}