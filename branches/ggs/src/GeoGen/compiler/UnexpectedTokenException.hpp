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
			CodeLocation location;
			std::string expectedTokenName;
			/*std::string actualTokenName;
			std::string actualTokenText;*/
		public:
			explicit UnexpectedTokenException(ErrorCode code, CodeLocation location, std::string const& expectedTokenName/*, std::string const& actualTokenName, std::string const& actualTokenText*/) :
				CompilerException(code), location(location), expectedTokenName(expectedTokenName)/*, actualTokenName(actualTokenName), actualTokenText(actualTokenText)*/
			{				
			};

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Unexpected token at line " << location.GetLine() << ", column " << location.GetColumn() << ", got " << expectedTokenName << "." << std::endl;
				return ss.str();
			}
		};
	}
}