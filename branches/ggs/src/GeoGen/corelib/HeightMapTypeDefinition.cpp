#include "HeightMapTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightMapFlatFunctionDefinition.hpp"
#include "HeightMapAddFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

HeightMapTypeDefinition::HeightMapTypeDefinition() : RendererTypeDefinition(GG_STR("HeightMap"))
{
	this->GetFunctionDefinitions().AddItem(new HeightMapFlatFunctionDefinition(this));
	this->GetFunctionDefinitions().AddItem(new HeightMapAddFunctionDefinition(this));
}