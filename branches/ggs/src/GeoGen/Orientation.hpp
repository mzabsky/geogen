#pragma once

#include "String.hpp"

namespace geogen
{
	enum Orientation
	{
		ORIENTATION_HORIZONTAL,
		ORIENTATION_VERTICAL
	};

	String OrientationToString(Orientation dimensions);
}