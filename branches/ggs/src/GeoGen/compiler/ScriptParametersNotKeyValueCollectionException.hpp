#pragma once

#include <string>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class ScriptParametersNotKeyValueCollectionException : public CompilerException
		{
		private:
		public:
			ScriptParametersNotKeyValueCollectionException(CodeLocation location) :
				CompilerException(GGE1402_ScriptParametersNotKeyValueCollection, location) {};

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Parameters section of script metadata and definition of individual parameters have to be key-value collections on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ".";

				return ss.str();
			}
		};
	}
}