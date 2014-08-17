#include "HeightMapTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"

#include "HeightMapAbsFunctionDefinition.hpp"
#include "HeightMapAddFunctionDefinition.hpp"
#include "HeightMapBlurFunctionDefinition.hpp"
#include "HeightMapClampHeightsFunctionDefinition.hpp"
#include "HeightMapCombineFunctionDefinition.hpp"
#include "HeightMapFillFunctionDefinition.hpp"
#include "HeightMapFillRectangleFunctionDefinition.hpp"

#include "HeightMapFlatFunctionDefinition.hpp"
#include "HeightMapNoiseFunctionDefinition.hpp"
#include "HeightMapRadialGradientFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

HeightMapTypeDefinition::HeightMapTypeDefinition() : RendererTypeDefinition(GG_STR("HeightMap"))
{
	this->GetFunctionDefinitions().AddItem(new HeightMapAbsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapAddFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapBlurFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapClampHeightsFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapCombineFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapFillFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapFillRectangleFunctionDefinition(this));

	this->GetStaticFunctionDefinitions().AddItem(new HeightMapFlatFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapRadialGradientFunctionDefinition(this));
	this->GetStaticFunctionDefinitions().AddItem(new HeightMapNoiseFunctionDefinition(this));
}