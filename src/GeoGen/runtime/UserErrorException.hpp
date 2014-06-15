#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		class UserErrorException : public RuntimeException
		{
		private:
			String formattedUserMessage;
			String unformattedUserMessage;
			std::vector<String> arguments;
		public:
			UserErrorException(CodeLocation location, String const& formattedUserMessage, String const& unformattedUserMessage, std::vector<String> const& arguments) :
				RuntimeException(GGE5000_UserError, location), formattedUserMessage(formattedUserMessage), unformattedUserMessage(unformattedUserMessage), arguments(arguments) {};

			inline String GetFormattedUserMessage() const { return this->formattedUserMessage; }

			inline String GetUnformattedUserMessage() const { return this->unformattedUserMessage; }

			inline std::vector<String> const& GetArguments() const { return this->arguments; }

			virtual String GetDetailMessage()
			{
				StringStream ss;
				ss << "Script triggered an error with message \"" << this->GetFormattedUserMessage() << "\" on line " << this->GetLocation().GetLine() << ", column " << this->GetLocation().GetColumn() << ".";

				return ss.str();
			};
		};
	}
}