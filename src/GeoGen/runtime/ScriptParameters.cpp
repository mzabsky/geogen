#include <algorithm>

#include "ScriptParameter.hpp"
#include "ScriptParameters.hpp"

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
	}

	this->mapWidth = min(this->maxMapWidth, width);
}

void ScriptParameters::SetMapHeight(unsigned height)
{
	if (height == MAP_SIZE_AUTOMATIC)
	{
		this->mapHeight = MAP_SIZE_AUTOMATIC;
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

void ScriptParameters::Serialize(IOStream& stream) const
{
	stream << "MapWidth: " << this->mapWidth << endl;
	stream << "DefaultMapWidth: " << this->defaultMapWidth << endl;
	stream << "MinMapWidth: " << this->minMapWidth << endl;
	stream << "MaxMapWidth: " << this->maxMapWidth << endl;
	
	stream << "MapHeight: " << this->mapHeight << endl;
	stream << "DefaultMapHeight: " << this->defaultMapHeight << endl;
	stream << "MinMapHeight: " << this->minMapHeight << endl;
	stream << "MaxMapHeight: " << this->maxMapHeight << endl;
	
	stream << "RenderOriginX: " << this->renderOriginX << endl;
	stream << "RenderOriginY: " << this->renderOriginY << endl;
	stream << "RenderWidth: " << this->renderWidth << endl;
	stream << "RenderHeight: " << this->renderHeight << endl;

	stream << "RenderScale: " << this->renderScale << endl;
	stream << "RandomSeed: " << this->randomSeed << endl;

	stream << "Parameters: " << endl;

	for (const_iterator it = this->Begin(); it != this->End(); it++)
	{		
		it->second->SerializeWithTabs(stream, 1);
	}
}