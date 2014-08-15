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
	this->defaultMapWidth = other.GetDefaultMapWidth();
	this->minMapWidth = other.GetMinMapWidth();
	this->maxMapWidth = other.GetMaxMapWidth();
	this->mapWidth = other.GetMapWidth();

	this->defaultMapHeight = other.GetDefaultMapHeight();
	this->minMapHeight = other.GetMinMapHeight();
	this->maxMapHeight = other.GetMaxMapHeight();
	this->mapHeight = other.GetMapHeight();

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

void ScriptParameters::Serialize(IOStream& stream) const
{
	stream << "{" << endl;
	stream << "\t" << "MapWidth: " << this->mapWidth << endl;
	stream << "\t" << "DefaultMapWidth: " << this->defaultMapWidth << endl;
	stream << "\t" << "MinMapWidth: " << this->minMapWidth << endl;
	stream << "\t" << "MaxMapWidth: " << this->maxMapWidth << endl;
	
	stream << "\t" << "MapHeight: " << this->mapHeight << endl;
	stream << "\t" << "DefaultMapHeight: " << this->defaultMapHeight << endl;
	stream << "\t" << "MinMapHeight: " << this->minMapHeight << endl;
	stream << "\t" << "MaxMapHeight: " << this->maxMapHeight << endl;
	
	stream << "\t" << "RenderOriginX: " << this->renderOriginX << endl;
	stream << "\t" << "RenderOriginY: " << this->renderOriginY << endl;
	stream << "\t" << "RenderWidth: " << this->renderWidth << endl;
	stream << "\t" << "RenderHeight: " << this->renderHeight << endl;

	stream << "\t" << "RenderScale: " << this->renderScale << endl;
	stream << "\t" << "RandomSeed: " << this->randomSeed << endl;

	stream << "\t" << "Parameters: " << endl;
	stream << "\t" << "{" << endl;

	for (const_iterator it = this->Begin(); it != this->End(); it++)
	{		
		it->second->SerializeWithTabs(stream, 1);
	}

	stream << "\t" << "}" << endl;
	stream << "}" << endl;
}