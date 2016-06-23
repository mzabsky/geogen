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

#include "VariableTable.hpp"
#include "ManagedObject.hpp"
#include "../InternalErrorException.hpp"

using namespace geogen;
using namespace geogen::runtime;
using namespace std;

VariableTable::~VariableTable()
{
	// Don't remove refs if the MM is already deleting everything anyways (could remove refs on already released object).
	if (this->memoryManager->IsInCleanupMode())
	{
		return;
	}

	for (std::map<String, VariableTableItem>::iterator it = this->table.begin(); it != this->table.end(); it++)
	{
		(*it).second.GetValue()->RemoveRef(*this->memoryManager);
	}
}

VariableTableItem const* VariableTable::GetVariable(String const& variableName) const
{
	std::map<String, VariableTableItem>::const_iterator item = this->table.find(variableName);

	if (item == this->table.end()){
		return NULL;
	}

	return &(*item).second;
};

VariableTableItem* VariableTable::GetVariable(String const& variableName)
{
	std::map<String, VariableTableItem>::iterator item = this->table.find(variableName);

	if (item == this->table.end()){
		return NULL;
	}

	return &(*item).second;
};

bool VariableTable::IsVariableDeclared(String const& symbolName) const
{
	return this->table.find(symbolName) != this->table.end();
};

bool VariableTable::DeclareVariable(String const& symbolName, ManagedObject* value, bool isConst) {
	if (value == NULL)
	{
		throw InternalErrorException(GG_STR("Can't declare variable with native NULL value (use VM null)."));
	}

	if (this->table.find(symbolName) != this->table.end()){
		return false;
	}

	// We know only const operations will be done with the object (if the collection is not exclusively owning).
	this->table[symbolName] = VariableTableItem(value, isConst);

	value->AddRef();

	return true;
};

void VariableTable::Serialize(IOStream& stream) const
{
	for (const_iterator it = this->Begin(); it != this->End(); it++)
	{
		stream << it->first << GG_STR(": ") << (it->second.IsConst() ? "const " : "");
		it->second.GetValue()->SerializeWithTabs(stream, 1);
	}
}