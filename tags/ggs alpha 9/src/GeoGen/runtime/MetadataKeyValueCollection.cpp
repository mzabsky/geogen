#include <algorithm>

#include "MetadataKeyValueCollection.hpp"

using namespace std;
using namespace geogen;
using namespace geogen::runtime;

void MetadataKeyValueCollection::MoveKeyValuesFrom(MetadataKeyValueCollection& another)
{
	std::move(another.table.begin(), another.table.end(), std::inserter(this->table, this->table.end()));

	another.table.clear();
}