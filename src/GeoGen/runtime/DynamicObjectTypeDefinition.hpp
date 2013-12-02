#pragma once

#include <string>

#include "../compiler/TypeDefinition.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class DynamicObjectTypeDefinition : public compiler::TypeDefinition
		{
		private:
			compiler::TypeDefinition const* type;
		public:
			DynamicObjectTypeDefinition() : compiler::TypeDefinition("DynamicObject") { }
		};
	}
}