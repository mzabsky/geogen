#pragma once

#include "RuntimeException.hpp"
#include "SymbolDefinitionException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE2201_UndefinedFunction, geogen::GGE2202_UndefinedVariable and geogen::GGE2203_UndefinedMemberVariable occurs.
		class UndefinedSymbolAccessException : public SymbolDefinitionException
		{
		public:

			/// Constructor.
			/// @param code The error code.
			/// @param location The code location.
			/// @param symbolName Name of the symbol.
			UndefinedSymbolAccessException(ErrorCode code, CodeLocation location, String const& symbolName) :
				SymbolDefinitionException(code, location, symbolName) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Undefined symbol \"" << this->GetSymbolName() << "\" accessed on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() <<  ".";
				return ss.str();
			}
		};
	}
}