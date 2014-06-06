#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class UserErrorException : public RuntimeException
		{
		private:
			String userMessage;
		public:
			UserErrorException(CodeLocation location, String const& userMessage) :
				RuntimeException(GGE5000_UserError, location), userMessage(userMessage) {};

			inline String GetUserMessage() const { return this->userMessage; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Script triggered an error with message \"" << this->GetUserMessage() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";

				return ss.str();
			};
		};
	}
}