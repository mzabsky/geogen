/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "CoordinateObject.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"
#include "../runtime/VirtualMachine.hpp"
#include "../Size.hpp"
#include "RelativeCoordinateInInfiniteDimensionException.hpp"
#include "CoordinateOverflowException.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

Coordinate CoordinateObject::GetAbsoluteCoordinate(runtime::VirtualMachine* vm, CodeLocation location, Direction direction) const
{
	if (!this->IsRelative())
	{
		return (Coordinate)this->GetValue();
	}

	Size1D mapSize;
	switch (direction)
	{
	case DIRECTION_HORIZONTAL:
		mapSize = vm->GetArguments().GetMapWidth();
		break;
	case DIRECTION_VERTICAL:
		mapSize = vm->GetArguments().GetMapHeight();
		break;
	default:
		throw InternalErrorException(GG_STR("Invalid Direction."));
	}

	if (mapSize == MAP_SIZE_INFINITE)
	{
		throw RelativeCoordinateInInfiniteDimensionException(location);
	}

	Number result = mapSize * this->GetValue();

	if (result > COORDINATE_MAX || result < COORDINATE_MIN)
	{
		throw CoordinateOverflowException(location);
	}

	return NumberToInt(result);
}