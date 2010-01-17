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

GGen_Path::GGen_Path(): closed(false) {}

void GGen_Path::AddPoint(GGen_Point* point){
	GGen_Script_Assert(point != NULL);
	
	this->points.push_back(point);
}

void GGen_Path::RemovePoint(uint32 index){
	GGen_Script_Assert(index < this->points.size());
	
	Iterator it = points.begin();
	for(unsigned i = 0; i < points.size(); i++){
		it++;
	}

	this->points.erase(it);
}