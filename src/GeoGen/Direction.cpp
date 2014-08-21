#include "Direction.hpp"
#include "InternalErrorException.hpp"

geogen::String geogen::DirectionToString(Direction dimensions)
{
	switch (dimensions)
	{
	case DIRECTION_HORIZONTAL: return GG_STR("Horizontal");
	case DIRECTION_VERTICAL: return GG_STR("Vertical");
	default:
		throw InternalErrorException(GG_STR("Invalid Direction."));
	}
}