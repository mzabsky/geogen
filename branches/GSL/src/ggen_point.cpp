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
#include "ggen_point.h"

namespace GeoGen{
	Point::Point(CoordOffset x, CoordOffset y): x(x), y(y) {
		ScriptAssert(Generator::GetInstance()->GetStatus() != GGEN_LOADING_MAP_INFO);
	}

	CoordOffset Point::GetX(){
		return this->x;
	}

	CoordOffset Point::GetY(){
		return this->y;
	}

	void Point::SetX(CoordOffset x){
		this->x = x;
	}

	void Point::SetY(CoordOffset y){
		this->y = y;
	}

	void Point::SetCoords(CoordOffset x, CoordOffset y){
		this->x = x;
		this->y = x;
	}
}