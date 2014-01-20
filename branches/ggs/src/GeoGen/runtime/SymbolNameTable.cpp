#include "SymbolNameTable.hpp"
#include "../InternalErrorException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void SymbolNameTable::AddName(std::string const& name) 
{ 
	std::vector<std::string>::iterator position = std::find(this->table.begin(), this->table.end(), name);
	if(position == this->table.end())
	{		
		this->table.push_back(name);
	}
};

int SymbolNameTable::GetNameIndex(std::string const& name) 
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
			
int SymbolNameTable::GetNameIndex(std::string const& name) const
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

std::string const& SymbolNameTable::GetNameByIndex(int symbolNameIndex) const
{
	if (symbolNameIndex >= (int)this->table.size()){
		throw InternalErrorException("Symbol index not present in symbol name table.");
	}

	return this->table[symbolNameIndex];
};

size_t SymbolNameTable::ContainsName(std::string const& name) const
{ 
	std::vector<std::string>::const_iterator position = std::find(this->table.begin(), this->table.end(), name);
	return position != this->table.end();
};

void SymbolNameTable::Serialize(std::iostream& stream) const
{
	int i = 0;
	for (const_iterator it = this->Begin(); it != this->End(); it++)
	{
		stream << i << " " << *it << std::endl;
		i++;
	}
}

