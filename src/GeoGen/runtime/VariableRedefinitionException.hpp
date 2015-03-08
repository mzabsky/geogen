#pragma once

#include "RuntimeException.hpp"
#include "SymbolDefinitionException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE2204_VariableAlreadyDefined occurs.
		class VariableRedefinitionException : public SymbolDefinitionException
		{
		public:
			VariableRedefinitionException(CodeLocation location, String const& symbolName) :
				SymbolDefinitionException(GGE2204_VariableAlreadyDefined, location, symbolName) {};

			virtual ~VariableRedefinitionException() throw () {}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Variable \"" << this->GetSymbolName() << "\" declared on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << " was already defined.";
				return ss.str();
			}
		};
	}
}