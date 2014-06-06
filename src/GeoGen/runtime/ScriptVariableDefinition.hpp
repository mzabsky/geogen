#pragma once

#include <vector>

#include "VariableDefinition.hpp"

namespace geogen 
{
	namespace runtime
	{
		class ScriptVariableDefinition : public VariableDefinition
		{
		private:
		public:
			ScriptVariableDefinition(String const& name) : VariableDefinition(name, false) 
			{
			}
		};
	}
}