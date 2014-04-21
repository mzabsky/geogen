#pragma once

#include <string>
#include <stdexcept>

#include "CompilerException.hpp"

namespace geogen
{
	namespace compiler
	{
		class UnexpectedTokenException : public CompilerException
		{
		private:
			std::string expectedTokenName;
			/*std::string actualTokenName;
			std::string actualTokenText;*/
		public:
			explicit UnexpectedTokenException(ErrorCode code, CodeLocation location, std::string const& expectedTokenName/*, std::string const& actualTokenName, std::string const& actualTokenText*/) :
				CompilerException(code, location), expectedTokenName(expectedTokenName)/*, actualTokenName(actualTokenName), actualTokenText(actualTokenText)*/
			{				
			};

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Unexpected token on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ", expected \"" << expectedTokenName << "\".";
				return ss.str();
			}
		};
	}
}