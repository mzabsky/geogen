#pragma once

#include "../String.hpp"
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

			virtual String GetDetailMessage()
			{
				return "Attempted to divide by 0.";
			}
		};
	}
}