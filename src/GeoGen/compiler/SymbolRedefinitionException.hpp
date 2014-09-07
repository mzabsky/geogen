#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when errors geogen::GGE1306_FunctionAlreadyDefined, geogen::GGE1308_TypeAlreadyDefined and geogen::GGE1309_EnumValueAlreadyDefined occur.
		class SymbolRedefinitionException : public CompilerException
		{
		private:
			String symbolName;
		public:

			/// Constructor.
			/// @param code The code.
			/// @param location The location.
			/// @param symbolName Name of the symbol.
			SymbolRedefinitionException(ErrorCode code, CodeLocation location, String const& symbolName) :
				CompilerException(code, location), symbolName(symbolName) {};

			/// Gets symbol name.
			/// @return The symbol name.
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