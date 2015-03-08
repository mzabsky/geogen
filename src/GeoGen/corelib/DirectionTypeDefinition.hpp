#pragma once

#include "EnumTypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		class DirectionTypeDefinition : public EnumTypeDefinition
		{
		private:
			DirectionTypeDefinition(ValueDefinitions valueDefinitions) : EnumTypeDefinition(GG_STR("Direction"), valueDefinitions) {};
		public:
			static DirectionTypeDefinition* Create();
		};
	}
}