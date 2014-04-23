#include "ScriptParameter.hpp"
#include "ScriptParameters.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

ScriptParameters::ScriptParameters(ScriptParameters const& other)
{
	for (const_iterator it = other.Begin(); it != other.End(); it++)
	{
		this->table[it->first] = it->second->Clone();
	}
}

ScriptParameters& ScriptParameters::operator=(ScriptParameters const& other)
{
	this->table.clear();

	for (const_iterator it = other.Begin(); it != other.End(); it++)
	{
		this->table[it->first] = it->second->Clone();
	}

	return *this;
}

void ScriptParameters::ResetToDefaults()
{
	for (iterator it = this->Begin(); it != this->End(); it++)
	{
		it->second->ResetToDefault();
	}
}