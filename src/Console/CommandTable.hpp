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

#include <vector>
#include <map>

#include <GeoGen/GeoGen.hpp>

namespace geogen
{
	namespace console
	{
		class Command;

		class CommandTable
		{
			std::map<String, Command*> table;
			std::vector<Command*> ownedCommands;

			// Non-copyable
			CommandTable(CommandTable const&) {};
			CommandTable& operator=(CommandTable const&) {};
		public:
			typedef std::vector<Command*>::const_iterator const_iterator;
			typedef std::vector<Command*>::iterator iterator;

			CommandTable() {};
			~CommandTable();

			inline const_iterator Begin() const { return this->ownedCommands.begin(); }
			inline const_iterator End() const { return this->ownedCommands.end(); }

			inline iterator Begin() { return this->ownedCommands.begin(); }
			inline iterator End() { return this->ownedCommands.end(); }

			void AddCommand(Command* command);
			Command const* GetCommand(String cue) const;
		};
	}
}