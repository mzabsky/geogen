#pragma once

#include <string>
#include <stdexcept>

#include "../runtime/RuntimeException.hpp"
#include "MathDefinitionRangeException.hpp"

namespace geogen
{
	namespace corelib
	{
		class DivisionByZeroException : public MathDefinitionRangeException
		{
		public:
			DivisionByZeroException(CodeLocation location) :
				MathDefinitionRangeException(GGE2301_DivisionByZero, location, "/", 0) {};

			virtual std::string GetDetailMessage()
			{
				return "Attempted to divide by 0.";
			}
		};
	}
}