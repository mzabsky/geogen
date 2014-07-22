#include "PointObject.hpp"
#include "../InternalErrorException.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../runtime/StaticObject.hpp"
#include "../Size.hpp"
#include "RelativeCoordinateInInfiniteDimensionException.hpp"
#include "CoordinateOverflowException.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

Point PointObject::GetAbsolutePoint(runtime::VirtualMachine* vm, CodeLocation location) const
{
	if (!this->IsXRelative() && !this->IsYRelative())
	{
		return Point((Coordinate)NumberToInt(this->GetX()), (Coordinate)NumberToInt(this->GetY()));
	}

	Number resultX, resultY;

	if (this->IsXRelative())
	{
		if (vm->GetArguments().GetMapWidth() == MAP_SIZE_INFINITE)
		{
			throw RelativeCoordinateInInfiniteDimensionException(location);
		}

		resultX = this->GetX() * vm->GetArguments().GetMapWidth();

		if (resultX > COORDINATE_MAX || resultX < COORDINATE_MIN)
		{
			throw CoordinateOverflowException(location);
		}
	}
	else {
		resultX = this->GetX();
	}
	
	if (this->IsYRelative())
	{
		if (vm->GetArguments().GetMapHeight() == MAP_SIZE_INFINITE)
		{
			throw RelativeCoordinateInInfiniteDimensionException(location);
		}

		resultY = this->GetY() * vm->GetArguments().GetMapHeight();

		if (resultY > COORDINATE_MAX || resultY < COORDINATE_MIN)
		{
			throw CoordinateOverflowException(location);
		}
	}
	else {
		resultY = this->GetY();
	}

	return Point(NumberToInt(resultX), NumberToInt(resultY));
}