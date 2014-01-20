#pragma once

#include "DynamicObject.hpp"
#include "EnumTypeDefinition.hpp"

namespace geogen
{
	namespace runtime
	{
		template<class T>
		class PrimitiveObject : public DynamicObject
		{
		private:
			T value;			
		public:
			PrimitiveObject(TypeDefinition const* type, T value) : DynamicObject(type), value(value)
			{				
			};
			inline T GetValue() const { return this->value; }
		};
	}
}