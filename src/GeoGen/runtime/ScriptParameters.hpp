#pragma once

#include <string>
#include <map>
#include "ScriptParameter.hpp"
#include "../utils/OwningMap.hpp"

namespace geogen
{
	namespace runtime
	{
		class ScriptParameters : public utils::OwningMap<ScriptParameter>
		{
		private:
		public:
			typedef std::map<std::string, ScriptParameter const*>::const_iterator const_iterator;
			typedef std::map<std::string, ScriptParameter*>::iterator iterator;

			ScriptParameters() {}

			void ResetToDefaults();
		};
	}
}