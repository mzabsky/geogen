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
		return this->GetValue();
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
		throw InternalErrorException("Invalid Direction.");
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