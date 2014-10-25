#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Base class for runtime exceptions related to a definition of a symbol.
		class SymbolDefinitionException : public RuntimeException
		{
		private:
			String symbolName;
		public:

			/// Constructor.
			/// @param code The code.
			/// @param location The location.
			/// @param symbolName Name of the symbol.
			explicit SymbolDefinitionException(ErrorCode code, CodeLocation location, String const& symbolName) :
				RuntimeException(code, location), symbolName(symbolName) {};

			/// Gets symbol name.
			/// @return The symbol name.
			inline String GetSymbolName() const { return this->symbolName; }

			virtual String GetDetailMessage() = 0;
		};
	}
}