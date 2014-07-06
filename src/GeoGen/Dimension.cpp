#include "Dimension.hpp"
#include "InternalErrorException.hpp"

geogen::String geogen::DimensionToString(Dimension dimensions)
{
	switch (dimensions)
	{
	case DIMENSION_WIDTH: return GG_STR("Width");
	case DIMENSION_HEIGHT: return GG_STR("Height");
	default:
		throw InternalErrorException(GG_STR("Invalid Dimension."));
	}
}