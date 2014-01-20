#pragma once

#include "PrimitiveObject.hpp"

namespace geogen
{
	namespace runtime
	{
		template<class T>
		class EnumObject : public PrimitiveObject<int>
		{
		private:
			T value;
		public:
			EnumObject(TypeDefinition const* type, T value)
			{

			}
			inline T GetValue() const { return this->value; }
		};
	}
}