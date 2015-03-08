#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE2108_ReadOnlyWrite occurs.
		class ReadOnlyWriteException : public RuntimeException
		{
		private:
			String symbolName;
		public:

			/// Constructor.
			/// @param location The location.
			/// @param symbolName Name of the symbol.
			explicit ReadOnlyWriteException(CodeLocation location, String const& symbolName) :
				RuntimeException(GGE2108_ReadOnlyWrite, location), symbolName(symbolName) {};

			/// Gets symbol name.
			/// @return The symbol name.
			inline String GetSymbolName() const { return this->symbolName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to write read only variable \"" << this->GetSymbolName() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}