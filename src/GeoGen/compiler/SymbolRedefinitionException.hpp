#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class SymbolRedefinitionException : public CompilerException
		{
		private:
			std::string symbolName;
		public:
			explicit SymbolRedefinitionException(ErrorCode code, std::string const& symbolName) :
				CompilerException(code), symbolName(symbolName) {};

			inline std::string GetSymbolName() const { return this->symbolName; }

			virtual std::string GetDetailMessage() 
			{ 
				return std::string("Symbol \"") + symbolName + "\" is already defined in its scope.";
			}
		};
	}
}