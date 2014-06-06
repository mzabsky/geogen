#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class InvalidSymbolDefinitionException : public CompilerException
		{
		private:
			String symbolName;
		public:
			explicit InvalidSymbolDefinitionException(ErrorCode code, CodeLocation location, String const& symbolName) :
				CompilerException(code, location), symbolName(symbolName) {};

			inline String GetSymbolName() const { return this->symbolName; }

			virtual String GetDetailMessage()
			{
				return String("Symbol \"") + symbolName + "\" has bad definition.";
			}
		};
	}
}