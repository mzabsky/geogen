#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE2103_IncorrectNumberOfArguments occurs.
		class NumberOfArgumentsException : public RuntimeException
		{
		private:
			unsigned expectedNumberOfArguments;
			unsigned actualNumberOfArguments;
		public:
			NumberOfArgumentsException(CodeLocation location, unsigned expectedNumberOfArguments, unsigned actualNumberOfArguments) :
				RuntimeException(GGE2103_IncorrectNumberOfArguments, location), expectedNumberOfArguments(expectedNumberOfArguments), actualNumberOfArguments(actualNumberOfArguments) {};

			inline unsigned GetExpectedNumberOfArguments() const { return this->expectedNumberOfArguments; }
			inline unsigned GetActualNumberOfArguments() const { return this->actualNumberOfArguments; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Incorrect number of arguments to a function call, " << this->GetExpectedNumberOfArguments() << " expected, got " << this->GetActualNumberOfArguments() << " on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}