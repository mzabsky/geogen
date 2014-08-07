#include "CommandTable.hpp"
#include "Command.hpp"
#include "../GeoGen/InternalErrorException.hpp"

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


