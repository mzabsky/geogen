#pragma once

#include <string>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class ScriptParameterNotKeyValueCollectionException : public CompilerException
		{
		private:
			std::string parameterName;
		public:
			ScriptParameterNotKeyValueCollectionException(CodeLocation location, std::string const& parameterName) :
				CompilerException(GGE1403_ScriptParameterNotKeyValueCollection, location), parameterName(parameterName) {};

			inline std::string GetParameterName() { return parameterName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Section of script metadata defining parameter \"" << parameterName << "\" has to be a key-value collection on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}