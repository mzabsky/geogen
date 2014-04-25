#pragma once

#include "ManagedObject.hpp"

namespace geogen
{
	namespace runtime
	{
		template<class T>
		class NativeObject : public ManagedObject
		{
		private:
			T* value;
		public:
			NativeObject(TypeDefinition const* type, T* value) : ManagedObject(type), value(value) {};
			virtual ~NativeObject() { delete value; };

			inline T* GetValue() const { return this->value; }
		};
	}
}