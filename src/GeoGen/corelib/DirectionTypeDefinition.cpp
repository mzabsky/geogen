#include "DirectionTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../Direction.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

DirectionTypeDefinition* DirectionTypeDefinition::Create()
{
	map<String, int> definitions;
	definitions[DirectionToString(DIRECTION_HORIZONTAL)] = (int)DIRECTION_HORIZONTAL;
	definitions[DirectionToString(DIRECTION_VERTICAL)] = (int)DIRECTION_VERTICAL;

	return new DirectionTypeDefinition(definitions);
}
