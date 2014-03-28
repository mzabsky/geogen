#include "VariableTable.hpp"

using namespace geogen::runtime;

VariableTableItem const* VariableTable::GetVariable(std::string const& variableName) const
{
	std::map<std::string, VariableTableItem>::const_iterator item = this->table.find(variableName);

	if (item == this->table.end()){
		return NULL;
	}

	return &(*item).second;
};

VariableTableItem* VariableTable::GetVariable(std::string const& variableName)
{
	std::map<std::string, VariableTableItem>::iterator item = this->table.find(variableName);

	if (item == this->table.end()){
		return NULL;
	}

	return &(*item).second;
};

bool VariableTable::SetVariable(std::string const& variableName, DynamicObject* value)
{
	std::map<std::string, VariableTableItem>::iterator item = this->table.find(variableName);

	if (item == this->table.end()){
		return false;
	}

	if (!(*item).second.SetValue(value))
	{
		return false;
	}

	return true;
};

bool VariableTable::IsVariableDeclared(std::string const& symbolName) const
{
	return this->table.find(symbolName) != this->table.end();
};

bool VariableTable::DeclareVariable(std::string const& symbolName, DynamicObject* value, bool isConst) {
	if (this->table.find(symbolName) != this->table.end()){
		return false;
	}

	// We know only const operations will be done with the object (if the collection is not exclusively owning).
	this->table[symbolName] = VariableTableItem(value, isConst);

	return true;
};