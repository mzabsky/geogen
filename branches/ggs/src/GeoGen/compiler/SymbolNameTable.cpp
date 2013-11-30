#include "SymbolNameTable.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::compiler;
//using namespace geogen_generated;

void SymbolNameTable::AddName(std::string name) 
{ 
	std::vector<std::string>::iterator position = std::find(this->table.begin(), this->table.end(), name);
	if(position == this->table.end())
	{		
		this->table.push_back(name);
	}
};

int SymbolNameTable::GetNameIndex(std::string name) 
{ 
	std::vector<std::string>::iterator position = std::find(this->table.begin(), this->table.end(), name);
	if(position != this->table.end())
	{
		return position - this->table.begin();
	}
	else 
	{
		this->table.push_back(name);
		return this->table.size() - 1;
	}
};
			
int SymbolNameTable::GetNameIndex(std::string name) const
{ 
	std::vector<std::string>::const_iterator position = std::find(this->table.begin(), this->table.end(), name);
	if(position != this->table.end())
	{
		return position - this->table.begin();
	}
	else 
	{
		return -1;
	}
};

size_t SymbolNameTable::ContainsName(std::string name) const
{ 
	std::vector<std::string>::const_iterator position = std::find(this->table.begin(), this->table.end(), name);
	return position != this->table.end();
};