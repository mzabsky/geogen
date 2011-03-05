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

namespace GeoGen{
	Path::Path() {
		ScriptAssert(Generator::GetInstance()->GetStatus() != GGEN_LOADING_MAP_INFO);
	}

	void Path::AddPoint(Point& point){
		ScriptAssert(this->points.size() + 1 < GGEN_MAX_PATH_LENGTH);

		this->points.push_back(point);
	}

	void Path::AddPointByCoords(CoordOffset x, CoordOffset y){
		ScriptAssert(this->points.size() + 1 < GGEN_MAX_PATH_LENGTH);
	
		this->points.push_back(Point(x, y));
	}

	void Path::RemovePoint(uint32 index){
		ScriptAssert(index < this->points.size());
	
		Iterator it = points.begin();
		for(unsigned i = 0; i < index; i++){
			it++;
		}

		this->points.erase(it);
	}

	void Path::InsertPoint(uint32 index, Point& point){
		ScriptAssert(this->points.size() + 1 < GGEN_MAX_PATH_LENGTH);
		ScriptAssert(index < this->points.size());
	
		Iterator it = points.begin();
		for(unsigned i = 0; i < index; i++){
			it++;
		}		

		points.insert(it, point);
	}

	void Path::InsertPointByCoords(uint32 index, CoordOffset x, CoordOffset y){
		ScriptAssert(this->points.size() + 1 < GGEN_MAX_PATH_LENGTH);
		ScriptAssert(index < this->points.size());
	
		Point temp(x, y);

		this->InsertPoint(index, temp);
	}

	void Path::Clear(){
		this->points.clear();
	}

	void Path::Move(CoordOffset x, CoordOffset y){
		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			(*i).x += x;
			(*i).y += y;
		}
	}

	void Path::Transform(CoordOffset origin_x, CoordOffset origin_y, double a11, double a12, double a21, double a22){
		/* Multiply the coordinate vector of each point by the transformation matrix */
		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			CoordOffset x = (*i).x;
			(*i).x = origin_x + (CoordOffset) ((double) (x - origin_x) * a11 + (double) ((*i).y - origin_y) * a12);
			(*i).y = origin_y + (CoordOffset) ((double) (x - origin_x) * a21 + (double) ((*i).y - origin_y) * a22);
		}	
	}

	void Path::Rotate(CoordOffset origin_x, CoordOffset origin_y, int32 angle)
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

	void Path::Shear(CoordOffset origin_x, CoordOffset origin_y, int32 horizontal_shear, int32 vertical_shear){
		this->Transform(origin_x, origin_y,
			1, 
			vertical_shear, 
			horizontal_shear, 
			1
		);
	}
 
	 void Path::Flip(CoordOffset origin_x, CoordOffset origin_y, Direction direction){
		this->Transform(origin_x, origin_y,
			direction == GGEN_HORIZONTAL ? 1 : -1, 
			0,
			0,
			direction == GGEN_VERTICAL ? 1 : -1
		);
	}

	 void Path::Scale(CoordOffset origin_x, CoordOffset origin_y, double ratio_x, double ratio_y){
		 ScriptAssert(ratio_x > 0 && ratio_y > 0);
	 
		 this->Transform(origin_x, origin_y,
			ratio_x, 
			0,
			0,
			ratio_y
		);
	}

	CoordOffset Path::GetMinX(){
		CoordOffset min = 30000;

		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			if((*i).x < min) min = (*i).x;
		}

		return min;
	}

	CoordOffset Path::GetMinY(){
		CoordOffset min = 30000;

		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			if((*i).y < min) min = (*i).y;
		}

		return min;
	}

	CoordOffset Path::GetMaxX(){
		CoordOffset max = -30000;

		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			if((*i).x > max) max = (*i).x;
		}

		return max;
	}

	CoordOffset Path::GetMaxY(){
		CoordOffset max = -30000;

		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			if((*i).y > max) max = (*i).y;
		}

		return max;
	}

	CoordOffset Path::GetCenterY(){
		CoordOffset max = -30000;
		CoordOffset min = 30000;

		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			if((*i).y > max) max = (*i).y;
			if((*i).y < min) min = (*i).y;
		}

		return (max - min) / 2;
	}

	CoordOffset Path::GetCenterX(){
		CoordOffset max = -30000;
		CoordOffset min = 30000;

		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			if((*i).x > max) max = (*i).x;
			if((*i).x < min) min = (*i).x;
		}

		return (max - min) / 2;
	}

	CoordOffset Path::GetAverageX(){
		int64 sum = 0;

		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			sum += (*i).x;
		}

		return (CoordOffset) (sum / this->points.size());
	}

	CoordOffset Path::GetAverageY(){
		int64 sum = 0;

		for(Path::Iterator i = this->points.begin(); i != this->points.end(); i++){
			sum += (*i).y;
		}

		return (CoordOffset) (sum / this->points.size());
	}
}