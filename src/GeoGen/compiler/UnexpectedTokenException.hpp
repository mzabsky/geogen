#pragma once

#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class UnexpectedTokenException : public CompilerException
		{
		private:
			String expectedTokenName;
			/*String actualTokenName;
			String actualTokenText;*/
		public:
			explicit UnexpectedTokenException(ErrorCode code, CodeLocation location, String const& expectedTokenName/*, String const& actualTokenName, String const& actualTokenText*/) :
				CompilerException(code, location), expectedTokenName(expectedTokenName)/*, actualTokenName(actualTokenName), actualTokenText(actualTokenText)*/
			{				
			};

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Unexpected token on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ", expected \"" << expectedTokenName << "\".";
				return ss.str();
			}
		};
	}
}