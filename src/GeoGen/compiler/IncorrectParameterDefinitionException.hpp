#pragma once

#include <string>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/*class IncorrectParameterDefinitionException : public CompilerException
		{
		private:
			std::string parameterName;
		public:
			IncorrectParameterDefinitionException(CodeLocation location, std::string const& parameterName) : 
				CompilerException(GGE1502_IncorrectParameterDefinition, location), parameterName(parameterName) {};

			inline std::string GetParameterName() { return this->parameterName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Parameter \"" << this->parameterName << "\" was incorrectly defined.";

				return ss.str();
			}
		};*/
	}
}