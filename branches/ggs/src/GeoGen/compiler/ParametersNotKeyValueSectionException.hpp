#pragma once

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class ParametersNotKeyValueSectionException : public CompilerException
		{
		private:
		public:
			ParametersNotKeyValueSectionException(CodeLocation location) :
				CompilerException(GGE1501_ParametersNotKeyValueSection, location) {};

			virtual std::string GetDetailMessage()
			{
				return "Parameters section of script metadata and definition of individual parameters have to be key-value collections.";
			}
		};
	}
}