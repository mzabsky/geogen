#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when errors geogen::GGE1310_EnumValueNotInteger and geogen::GGE1311_ReservedTypeNameUsed occur.
		class InvalidSymbolDefinitionException : public CompilerException
		{
		private:
			String symbolName;
		public:

			/// Constructor.
			/// @param code The code.
			/// @param location The location.
			/// @param symbolName Name of the symbol.
			explicit InvalidSymbolDefinitionException(ErrorCode code, CodeLocation location, String const& symbolName) :
				CompilerException(code, location), symbolName(symbolName) {};

			/// Gets symbol name.
			/// @return The symbol name.
			inline String GetSymbolName() const { return this->symbolName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Symbol \"" << symbolName << "\" has bad definition.";
				return ss.str();
			}
		};
	}
}