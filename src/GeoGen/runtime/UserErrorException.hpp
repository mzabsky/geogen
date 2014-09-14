#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE5000_UserError occurs.
		class UserErrorException : public RuntimeException
		{
		private:
			String formattedUserMessage;
			String unformattedUserMessage;
			std::vector<String> arguments;
		public:

			/// Constructor.
			/// @param location The code location.
			/// @param formattedUserMessage Formatted user message.
			/// @param unformattedUserMessage Unformatted user message.
			/// @param arguments Formatting arguments.
			UserErrorException(CodeLocation location, String const& formattedUserMessage, String const& unformattedUserMessage, std::vector<String> const& arguments) :
				RuntimeException(GGE5000_UserError, location), formattedUserMessage(formattedUserMessage), unformattedUserMessage(unformattedUserMessage), arguments(arguments) {};

			/// Gets formatted user message.
			/// @return The formatted user message.
			inline String GetFormattedUserMessage() const { return this->formattedUserMessage; }

			/// Gets unformatted user message.
			/// @return The unformatted user message.
			inline String GetUnformattedUserMessage() const { return this->unformattedUserMessage; }

			/// Gets the formatting arguments.
			/// @return The arguments.
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