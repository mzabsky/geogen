#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class SymbolRedefinitionException : public CompilerException
		{
		private:
			String symbolName;
		public:
			SymbolRedefinitionException(ErrorCode code, CodeLocation location, String const& symbolName) :
				CompilerException(code, location), symbolName(symbolName) {};

			inline String GetSymbolName() const { return this->symbolName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Symbol \"" << symbolName << "\" defined on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << " is already defined in its scope.";

				return ss.str();
			}
		};
	}
}