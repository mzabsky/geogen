/* GeoGen - Programmable height map generator
Copyright (C) 2015  Matej Zabsky

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include "RuntimeException.hpp"

namespace geogen
{
	namespace runtime
	{
		/// Exception thrown when error geogen::GGE6000_UserError occurs.
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
				RuntimeException(GGE6000_UserError, location), formattedUserMessage(formattedUserMessage), unformattedUserMessage(unformattedUserMessage), arguments(arguments) {};

			virtual ~UserErrorException() throw () {}

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