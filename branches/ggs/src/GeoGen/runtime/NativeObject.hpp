#pragma once

#include "DynamicObject.hpp"

namespace geogen
{
	namespace runtime
	{
		template<class T>
		class NativeObject : public DynamicObject
		{
		private:
			T* value;
		public:
			NumberObject(TypeDefinition const* type, T* value) : DynamicObject(type), value(value) {};
			inline T* GetValue() const { return this->value; }
		};
	}
}