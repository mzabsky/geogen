#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class ReadOnlyWriteException : public RuntimeException
		{
		private:
			String symbolName;
		public:
			explicit ReadOnlyWriteException(CodeLocation location, String const& symbolName) :
				RuntimeException(GGE2108_ReadOnlyWrite, location), symbolName(symbolName) {};

			inline String GetSymbolName() const { return this->symbolName; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Attempted to write read only variable \"" << this->GetSymbolName() + "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}