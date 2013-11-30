#pragma once

#include <string>
#include <stdexcept>

namespace geogen 
{
	namespace compiler 
	{
		class CompilerException : std::runtime_error
		{
		public:
			explicit CompilerException(const std::string& error_message)  :
				std::runtime_error(error_message) {};
		};
	}
}