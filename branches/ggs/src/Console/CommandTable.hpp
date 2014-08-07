#pragma once

#include <vector>
#include <map>

#include "../GeoGen/String.hpp"

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

			inline const_iterator Begin() const { return *(const_iterator*)(&this->ownedCommands.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->ownedCommands.end()); }

			inline iterator Begin() { return this->ownedCommands.begin(); }
			inline iterator End() { return this->ownedCommands.end(); }

			void AddCommand(Command* command);
			Command const* GetCommand(String cue) const;
		};
	}
}