#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class InvalidSymbolDefinitionException : public CompilerException
		{
		private:
			std::string symbolName;
		public:
			explicit InvalidSymbolDefinitionException(ErrorCode code, CodeLocation location, std::string const& symbolName) :
				CompilerException(code, location), symbolName(symbolName) {};

			inline std::string GetSymbolName() const { return this->symbolName; }

			virtual std::string GetDetailMessage()
			{
				return std::string("Symbol \"") + symbolName + "\" has bad definition.";
			}
		};
	}
}