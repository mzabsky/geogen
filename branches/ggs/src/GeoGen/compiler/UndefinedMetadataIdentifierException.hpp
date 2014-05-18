#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class UndefinedMetadataIdentifierException : public CompilerException
		{
		private:
			std::string identifier;
		public:
			UndefinedMetadataIdentifierException(CodeLocation location, std::string const& identifier) :
				CompilerException(GGE1410_UndefinedMetadataIdentifier, location), identifier(identifier) {};

			inline std::string GetIdentifier() const { return this->identifier; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Undefined identifier \"" << identifier << "\" refernced in script metadata on line " << GetLocation().GetLine() << ", column " << GetLocation().GetColumn() << ". Expected \"Number\", \"Boolean\" or \"Enum\".";

				return ss.str();
			}
		};
	}
}