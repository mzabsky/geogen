#pragma once

#include <string>
#include <map>
#include "ScriptParameter.hpp" // Necessary to properly compile the owning map.
#include "../utils/OwningMap.hpp"

namespace geogen
{	
	namespace runtime
	{
		class ScriptParameter;

		class ScriptParameters : public utils::OwningMap<ScriptParameter>
		{
		private:
		public:
			typedef std::map<std::string, ScriptParameter const*>::const_iterator const_iterator;
			typedef std::map<std::string, ScriptParameter*>::iterator iterator;

			ScriptParameters() {}
			ScriptParameters(ScriptParameters const&);
			ScriptParameters& operator=(ScriptParameters const&);

			void ResetToDefaults();
		};
	}
}