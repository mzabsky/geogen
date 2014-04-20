#include "ScriptParameters.hpp"

using namespace std;
using namespace geogen;
using namespace runtime;

void ScriptParameters::ResetToDefaults()
{
	for (iterator it = this->Begin(); it != this->End(); it++)
	{
		it->second->ResetToDefault();
	}
}