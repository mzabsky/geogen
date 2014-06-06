#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class InvalidEscapeSequenceException : public CompilerException
		{
		private:
			String sequence;
		public:
			explicit InvalidEscapeSequenceException(CodeLocation location, String const& sequence) :
				CompilerException(GGE1101_InvalidEscapeSequence, location), sequence(sequence)
			{
			};

			inline String GetEscapeSequence() const
			{
				return this->sequence;
			}

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Invalid escape sequence \"" << this->GetEscapeSequence() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}