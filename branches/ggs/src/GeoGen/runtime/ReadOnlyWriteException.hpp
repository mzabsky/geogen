#pragma once

#include <string>
#include <stdexcept>

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class ReadOnlyWriteException : public RuntimeException
		{
		private:
			std::string symbolName;
		public:
			explicit ReadOnlyWriteException(CodeLocation location, std::string const& symbolName) :
				RuntimeException(GGE2108_ReadOnlyWrite, location), symbolName(symbolName) {};

			inline std::string GetSymbolName() const { return this->symbolName; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Attempted to write read only variable \"" << this->GetSymbolName() + "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";
				return ss.str();
			}
		};
	}
}