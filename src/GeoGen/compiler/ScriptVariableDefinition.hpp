#pragma once

#include <string>
#include <vector>

#include "VariableDefinition.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class ScriptVariableDefinition : public VariableDefinition
		{
		private:
		public:
			ScriptVariableDefinition(std::string const& name) : VariableDefinition(name) 
			{
			}
		};
	}
}