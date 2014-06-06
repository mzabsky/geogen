#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class SymbolDefinitionException : public RuntimeException
		{
		private:
			String symbolName;
		public:
			explicit SymbolDefinitionException(ErrorCode code, CodeLocation location, String const& symbolName) :
				RuntimeException(code, location), symbolName(symbolName) {};

			inline String GetSymbolName() const { return this->symbolName; }

			virtual String GetDetailMessage() = 0;
		};
	}
}