#pragma once

#include <string>

#include "TypeDefinition.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class DynamicObjectTypeDefinition : public TypeDefinition
		{
		private:
			TypeDefinition const* type;
		public:
			DynamicObjectTypeDefinition() : TypeDefinition("DynamicObject") { }
		};
	}
}