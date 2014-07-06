#include "Orientation.hpp"
#include "InternalErrorException.hpp"

geogen::String geogen::OrientationToString(Orientation dimensions)
{
	switch (dimensions)
	{
	case ORIENTATION_HORIZONTAL: return GG_STR("Horizontal");
	case ORIENTATION_VERTICAL: return GG_STR("Vertical");
	default:
		throw InternalErrorException(GG_STR("Invalid Orientation."));
	}
}