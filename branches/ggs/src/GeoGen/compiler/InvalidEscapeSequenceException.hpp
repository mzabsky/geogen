#pragma once

#include "../String.hpp"
#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		/// Exception thrown when error geogen::GGE1102_InvalidEscapeSequence occurs.
		class InvalidEscapeSequenceException : public CompilerException
		{
		private:
			String sequence;
		public:

			/// Constructor.
			/// @param location The location.
			/// @param sequence The sequence.
			explicit InvalidEscapeSequenceException(CodeLocation location, String const& sequence) :
				CompilerException(GGE1102_InvalidEscapeSequence, location), sequence(sequence)
			{
			};

			/// Gets escape sequence.
			/// @return The escape sequence.
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