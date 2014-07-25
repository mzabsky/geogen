#include "HeightMapTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapFlatFunctionDefinition.hpp"
#include "HeightMapAddFunctionDefinition.hpp"
#include "HeightMapRadialGradientFunctionDefinition.hpp"
#include "HeightMapBlurFunctionDefinition.hpp"

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
}