#pragma once

#include <string>
#include <stdexcept>

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class DivisionByZeroException : public RuntimeException
		{
		public:
			DivisionByZeroException(CodeLocation location) :
				RuntimeException(GGE2301_DivisionByZero, location) {};

			virtual std::string GetDetailMessage()
			{
				return "Attempted to divide by 0.";
			}
		};
	}
}