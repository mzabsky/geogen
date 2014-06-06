#include "VariableTable.hpp"
#include "ManagedObject.hpp"
#include "..\InternalErrorException.hpp"

using namespace geogen;
using namespace geogen::runtime;

VariableTable::~VariableTable()
{
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
		throw InternalErrorException("Can't declare variable with native NULL value (use VM null).");
	}

	if (this->table.find(symbolName) != this->table.end()){
		return false;
	}

	// We know only const operations will be done with the object (if the collection is not exclusively owning).
	this->table[symbolName] = VariableTableItem(value, isConst);

	value->AddRef();

	return true;
};