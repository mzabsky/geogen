#pragma once

#include "RuntimeException.hpp"
#include "SymbolDefinitionException.hpp"

namespace geogen
{
	namespace runtime
	{
		class UndefinedSymbolAccessException : public SymbolDefinitionException
		{
		public:
			UndefinedSymbolAccessException(ErrorCode code, CodeLocation location, String const& symbolName) :
				SymbolDefinitionException(code, location, symbolName) {};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Undefined symbol \"" << this->GetSymbolName() + "\" accessed on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() <<  ".";
				return ss.str();
			}
		};
	}
}