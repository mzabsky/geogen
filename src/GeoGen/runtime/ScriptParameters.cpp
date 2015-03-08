#include <algorithm>

#include "ScriptParameter.hpp"
#include "ScriptParameters.hpp"
#include "../ApiUsageException.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

ScriptParameters::ScriptParameters(ScriptParameters const& other)
{
	this->defaultMapWidth = other.defaultMapWidth;
	this->minMapWidth = other.minMapWidth;
	this->maxMapWidth = other.maxMapWidth;
	this->mapWidth = other.mapWidth;

	this->defaultMapHeight = other.defaultMapHeight;
	this->minMapHeight = other.minMapHeight;
	this->maxMapHeight = other.maxMapHeight;
	this->mapHeight = other.mapHeight;

	this->renderOriginX = other.renderOriginX;
	this->renderOriginY = other.renderOriginY;
	this->renderWidth = other.renderWidth;
	this->renderHeight = other.renderHeight;
	this->renderScale = other.renderScale;

	this->randomSeed = other.randomSeed;

	this->enabledMaps = other.enabledMaps;

	for (const_iterator it = other.Begin(); it != other.End(); it++)
	{
		this->table[it->first] = it->second->Clone();
	}
}

ScriptParameters& ScriptParameters::operator=(ScriptParameters const& other)
{
	this->defaultMapWidth = other.defaultMapWidth;
	this->minMapWidth = other.minMapWidth;
	this->maxMapWidth = other.maxMapWidth;
	this->mapWidth = other.mapWidth;

	this->defaultMapHeight = other.defaultMapHeight;
	this->minMapHeight = other.minMapHeight;
	this->maxMapHeight = other.maxMapHeight;
	this->mapHeight = other.mapHeight;

	this->renderOriginX = other.renderOriginX;
	this->renderOriginY = other.renderOriginY;
	this->renderWidth = other.renderWidth;
	this->renderHeight = other.renderHeight;
	this->renderScale = other.renderScale;

	this->randomSeed = other.randomSeed;

	this->enabledMaps = other.enabledMaps;

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
		this->mapWidth = MAP_SIZE_AUTOMATIC;
		return;
	}

	this->mapWidth = min(this->maxMapWidth, width);
}

void ScriptParameters::SetMapHeight(unsigned height)
{
	if (height == MAP_SIZE_AUTOMATIC)
	{
		this->mapHeight = MAP_SIZE_AUTOMATIC;
		return;
	}

	this->mapHeight = min(this->maxMapHeight, height);
}

bool ScriptParameters::IsMapInfinite(Direction direction) const
{
	switch (direction)
	{
	case DIRECTION_HORIZONTAL:
		return this->GetMaxMapWidth() == MAP_SIZE_INFINITE;
	case DIRECTION_VERTICAL:
		return this->GetMaxMapHeight() == MAP_SIZE_INFINITE;
	default:
		throw ApiUsageException(GG_STR("Invalid direction."));
	}
}

void ScriptParameters::ResetToDefaults()
{
	for (iterator it = this->Begin(); it != this->End(); it++)
	{
		it->second->ResetToDefault();
	}
}

void ScriptParameters::Serialize(IOStream& stream) const
{
	stream << "MapWidth: " << this->mapWidth << GG_STR(" (") << this->GetMapWidth() << GG_STR(")") << endl;
	stream << "DefaultMapWidth: " << this->defaultMapWidth << GG_STR(" (") << this->GetDefaultMapWidth() << GG_STR(")") << endl;
	stream << "MinMapWidth: " << this->minMapWidth << GG_STR(" (") << this->GetMinMapWidth() << GG_STR(")") << endl;
	stream << "MaxMapWidth: " << this->maxMapWidth << GG_STR(" (") << this->GetMaxMapWidth() << GG_STR(")") << endl;
	
	stream << "MapHeight: " << this->mapHeight << GG_STR(" (") << this->GetMapHeight() << GG_STR(")") << endl;
	stream << "DefaultMapHeight: " << this->defaultMapHeight << GG_STR(" (") << this->GetDefaultMapHeight() << GG_STR(")") << endl;
	stream << "MinMapHeight: " << this->minMapHeight << GG_STR(" (") << this->GetMinMapHeight() << GG_STR(")") << endl;
	stream << "MaxMapHeight: " << this->maxMapHeight << GG_STR(" (") << this->GetMaxMapHeight() << GG_STR(")") << endl;
	
	stream << "RenderOriginX: " << this->renderOriginX << endl;
	stream << "RenderOriginY: " << this->renderOriginY << endl;
	stream << "RenderWidth: " << this->renderWidth << GG_STR(" (") << this->GetRenderWidth() << GG_STR(")") << endl;
	stream << "RenderHeight: " << this->renderHeight << GG_STR(" (") << this->GetRenderHeight() << GG_STR(")") << endl;

	stream << "RenderScale: " << this->renderScale << endl;
	stream << "RandomSeed: " << this->randomSeed << endl;

	stream << "Parameters: " << endl;

	for (const_iterator it = this->Begin(); it != this->End(); it++)
	{		
		stream << GG_STR("\t") << it->first << GG_STR(":") << endl;
		it->second->SerializeWithTabs(stream, 2);
	}
}