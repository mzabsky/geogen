#pragma once

#include <string>
#include <stdexcept>

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class UserErrorException : public RuntimeException
		{
		private:
			std::string userMessage;
		public:
			UserErrorException(CodeLocation location, std::string const& userMessage) :
				RuntimeException(GGE5000_UserError, location), userMessage(userMessage) {};

			inline std::string GetUserMessage() const { return this->userMessage; }

			virtual std::string GetDetailMessage()
			{
				std::stringstream ss;
				ss << "Script triggered an error with message \"" << this->GetUserMessage() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";

				return ss.str();
			};
		};
	}
}