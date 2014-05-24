#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class UnexpectedCharacterException : public CompilerException
		{
		private:
			char character;
		public:
			explicit UnexpectedCharacterException(CodeLocation location, char character) :
				CompilerException(GGE1101_UnexpectedCharacter, location), character(character)
			{
			};

			inline char GetCharacter() const 
			{
				return this->character;
			}

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Unexpected character \"" << this->GetCharacter() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}