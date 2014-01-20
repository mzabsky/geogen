#pragma once

#include <string>
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
			ScriptVariableDefinition(std::string const& name) : VariableDefinition(name, false) 
			{
			}
		};
	}
}