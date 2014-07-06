#include <algorithm>

#include "ScriptParameter.hpp"
#include "ScriptParameters.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

ScriptParameters::ScriptParameters(ScriptParameters const& other)
{
	this->defaultMapWidth = other.GetDefaultMapWidth();
	this->minMapWidth = other.GetMinMapWidth();
	this->maxMapWidth = other.GetMaxMapWidth();
	this->mapWidth = other.GetMapWidth();

	this->defaultMapHeight = other.GetDefaultMapHeight();
	this->minMapHeight = other.GetMinMapHeight();
	this->maxMapHeight = other.GetMaxMapHeight();
	this->mapHeight = other.GetMapHeight();

	for (const_iterator it = other.Begin(); it != other.End(); it++)
	{
		this->table[it->first] = it->second->Clone();
	}
}

ScriptParameters& ScriptParameters::operator=(ScriptParameters const& other)
{
	this->defaultMapWidth = other.GetDefaultMapWidth();
	this->minMapWidth = other.GetMinMapWidth();
	this->maxMapWidth = other.GetMaxMapWidth();
	this->mapWidth = other.GetMapWidth();

	this->defaultMapHeight = other.GetDefaultMapHeight();
	this->minMapHeight = other.GetMinMapHeight();
	this->maxMapHeight = other.GetMaxMapHeight();
	this->mapHeight = other.GetMapHeight();

	this->table.clear();

	for (const_iterator it = other.Begin(); it != other.End(); it++)
	{
		this->table[it->first] = it->second->Clone();
	}

	return *this;
}

void ScriptParameters::SetRenderScale(double renderScale)
{
	this->renderScale = std::min(std::max(renderScale, RENDER_SCALE_MIN), RENDER_SCALE_MAX);
}

void ScriptParameters::SetMapWidth(unsigned width)
{
	if (width == MAP_SIZE_AUTOMATIC)
	{
		this->mapWidth = this->defaultMapWidth;
	}

	this->mapWidth = min(this->maxMapWidth, width);
}

void ScriptParameters::SetMapHeight(unsigned height)
{
	if (height == MAP_SIZE_AUTOMATIC)
	{
		this->mapHeight = this->defaultMapHeight;
	}

	this->mapHeight = min(this->maxMapHeight, height);
}

void ScriptParameters::ResetToDefaults()
{
	for (iterator it = this->Begin(); it != this->End(); it++)
	{
		it->second->ResetToDefault();
	}
}