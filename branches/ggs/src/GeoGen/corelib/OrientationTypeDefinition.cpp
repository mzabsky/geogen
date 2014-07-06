#include "OrientationTypeDefinition.hpp"
#include "../runtime/ManagedObject.hpp"
#include "../Orientation.hpp"

using namespace geogen;
using namespace corelib;
using namespace runtime;
using namespace std;

OrientationTypeDefinition* OrientationTypeDefinition::Create()
{
	map<String, int> definitions;
	definitions[OrientationToString(ORIENTATION_HORIZONTAL)] = (int)ORIENTATION_HORIZONTAL;
	definitions[OrientationToString(ORIENTATION_VERTICAL)] = (int)ORIENTATION_VERTICAL;

	return new OrientationTypeDefinition(definitions);
}
