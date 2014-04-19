#include <algorithm>

#include "MetadataKeyValueCollection.hpp"
#include "..\ApiUsageException.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void MetadataKeyValueCollection::MoveKeyValuesFrom(MetadataKeyValueCollection& another)
{
	std::move(another.table.begin(), another.table.end(), std::inserter(this->table, this->table.end()));

	//iterator a = this->table.end();

	another.table.clear();
}

bool MetadataKeyValueCollection::AddItem(std::string const& name, MetadataValue* value) {
	if (this->table.find(name) != this->table.end()){
		return false;
	}

	this->table[name] = value;

	return true;
};

MetadataValue const* MetadataKeyValueCollection::GetItem(string const& key) const
{
	MetadataKeyValueCollection::const_iterator it = this->FindItem(key);
	if (it == this->End())
	{
		throw ApiUsageException("Could not find metadata item " + key + ".");
	}

	return it->second;
}

MetadataValue* MetadataKeyValueCollection::GetItem(string const& key)
{
	MetadataKeyValueCollection::iterator it = this->FindItem(key);
	if (it == this->End())
	{
		throw ApiUsageException("Could not find metadata item " + key + ".");
	}

	return it->second;
}

MetadataKeyValueCollection::~MetadataKeyValueCollection()
{
	/*for (std::map<std::string, MetadataValue*>::iterator it = this->table.begin(); it != this->table.end(); it++)
	{
		//delete it->second;
	}*/
}
