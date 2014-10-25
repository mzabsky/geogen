#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1410_UndefinedMetadataIdentifier occurs.
		class UndefinedMetadataIdentifierException : public CompilerException
		{
		private:
			String identifier;
		public:
			UndefinedMetadataIdentifierException(CodeLocation location, String const& identifier) :
				CompilerException(GGE1410_UndefinedMetadataIdentifier, location), identifier(identifier) {};

			inline String GetIdentifier() const { return this->identifier; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Undefined identifier \"" << identifier << "\" refernced in script metadata on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ". Expected \"Number\", \"Boolean\" or \"Enum\".";

				return ss.str();
			}
		};
	}
}