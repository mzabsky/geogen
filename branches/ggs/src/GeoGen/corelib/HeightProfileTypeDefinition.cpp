#include "HeightProfileTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"
#include "HeightProfileFlatFunctionDefinition.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

HeightProfileTypeDefinition::HeightProfileTypeDefinition() : RendererTypeDefinition(GG_STR("HeightProfile")) 
{
	this->GetFunctionDefinitions().AddItem(new HeightProfileFlatFunctionDefinition(this));
}