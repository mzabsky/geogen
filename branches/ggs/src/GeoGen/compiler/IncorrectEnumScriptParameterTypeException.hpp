#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class IncorrectEnumScriptParameterTypeException : public CompilerException
		{
		private:
			std::string parameterName;
			std::string typeName;
		public:
			IncorrectEnumScriptParameterTypeException(CodeLocation location, std::string const& parameterName, std::string typeName) :
				CompilerException(GGE1413_IncorrectEnumScriptParameterType, location), parameterName(parameterName), typeName(typeName) {};

			inline std::string GetParameterName() const { return this->parameterName; }

			inline std::string GetTypeName() const { return this->typeName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Enum script parameter \"" << parameterName << "\" references type \"" << typeName << "\" which is not a proper enum type on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}