#pragma once

#include "TypeDefinition.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class ObjectTypeDefinition : public TypeDefinition
		{
		private:
			TypeDefinition const* type;
		public:
			ObjectTypeDefinition() : TypeDefinition(GG_STR("Object")) { }
		};
	}
}