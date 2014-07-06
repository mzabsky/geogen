#pragma once

#include "EnumTypeDefinition.hpp"

namespace geogen
{
	namespace corelib
	{
		class OrientationTypeDefinition : public EnumTypeDefinition
		{
		private:
			OrientationTypeDefinition(ValueDefinitions valueDefinitions) : EnumTypeDefinition(GG_STR("Orientation"), valueDefinitions) {};
		public:
			static OrientationTypeDefinition* Create();
		};
	}
}