#pragma once

#include <string>
#include <stdexcept>

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class SymbolDefinitionException : public RuntimeException
		{
		private:
			std::string symbolName;
		public:
			explicit SymbolDefinitionException(ErrorCode code, CodeLocation location, std::string const& symbolName) :
				RuntimeException(code, location), symbolName(symbolName) {};

			inline std::string GetSymbolName() const { return this->symbolName; }

			virtual std::string GetDetailMessage() = 0;
		};
	}
}