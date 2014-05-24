#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class InvalidEscapeSequenceException : public CompilerException
		{
		private:
			std::string  sequence;
		public:
			explicit InvalidEscapeSequenceException(CodeLocation location, std::string const& sequence) :
				CompilerException(GGE1101_InvalidEscapeSequence, location), sequence(sequence)
			{
			};

			inline std::string GetEscapeSequence() const
			{
				return this->sequence;
			}

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Invalid escape sequence \"" << this->GetEscapeSequence() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}