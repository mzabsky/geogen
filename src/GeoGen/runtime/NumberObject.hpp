#pragma once

#include "../Number.hpp"
#include "DynamicObject.hpp"

namespace geogen
{
	namespace runtime
	{
		class NumberObject : public DynamicObject
		{
		private:
			Number value;			
		public:
			NumberObject(TypeDefinition const* type, Number value) : DynamicObject(type) {};
			inline Number GetValue() const { this->value; }
		};
	}
}