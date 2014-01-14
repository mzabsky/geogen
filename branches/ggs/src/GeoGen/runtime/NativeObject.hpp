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
			NativeObject(TypeDefinition const* type, T* value) : DynamicObject(type), value(value) {};
			virtual ~NativeObject() { delete value; };

			inline T* GetValue() const { return this->value; }
		};
	}
}