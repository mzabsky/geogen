#include "HeightMapTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapFlatFunctionDefinition.hpp"
#include "HeightMapAddFunctionDefinition.hpp"
#include "HeightMapRadialGradientFunctionDefinition.hpp"
#include "HeightMapBlurFunctionDefinition.hpp"
#include "HeightMapFillFunctionDefinition.hpp"
#include "HeightMapFillRectangleFunctionDefinition.hpp"
#include "HeightMapNoiseFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

HeightMapTypeDefinition::HeightMapTypeDefinition() : RendererTypeDefinition(GG_STR("HeightMap"))
{
	this->GetFunctionDefinitions().AddItem(new HeightMapFlatFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapAddFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapRadialGradientFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapBlurFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapFillFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapFillRectangleFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapNoiseFunctionDefinition(this));
}