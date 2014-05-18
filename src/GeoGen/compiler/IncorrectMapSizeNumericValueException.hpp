#pragma once

#include <string>

#include "CompilerException.hpp"
#include "../Number.hpp"
#include "../runtime/ScriptParameters.hpp"

namespace geogen
{
	namespace compiler
	{
		class IncorrectMapSizeNumericValueException : public CompilerException
		{
		private:
			Number actualValue;
		public:
			IncorrectMapSizeNumericValueException(CodeLocation location, Number actualValue) :
				CompilerException(GGE1411_IncorrectMapSizeNumericValue, location), actualValue(actualValue) {};

			inline Number GetActualValue() const { return this->actualValue; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Map size has to be a positive integer greater than or equal to " << runtime::MAP_SIZE_MIN << ", less than or equal to " << runtime::MAP_SIZE_MAX << ", was \"" << actualValue << "\" on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}