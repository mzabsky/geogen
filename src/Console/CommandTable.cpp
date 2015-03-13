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

#include "CommandTable.hpp"
#include "Command.hpp"

#include <GeoGen/GeoGen.hpp>

using namespace geogen;
using namespace console;

CommandTable::~CommandTable()
{
	for (std::vector<Command*>::iterator it = this->ownedCommands.begin(); it != this->ownedCommands.end(); it++)
	{
		delete *it;
	}
}

void CommandTable::AddCommand(Command* command)
{
	this->ownedCommands.push_back(command);

	for (std::vector<String>::const_iterator it = command->GetCues().begin(); it != command->GetCues().end(); it++)
	{
		if (!this->table.insert(std::pair<String, Command*>(*it, command)).second)
		{
			throw InternalErrorException("Command with the same cue already existed in the command table.");
		}
	}
};

Command const* CommandTable::GetCommand(String cue) const
{
	std::map<String, Command*>::const_iterator it = this->table.find(cue);

	if (it == this->table.end())
	{
		return NULL;
	}

	return it->second;
}


