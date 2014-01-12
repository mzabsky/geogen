#pragma once

#include <string>
#include <functional>

#include "TypeDefinition.hpp"
#include "VirtualMachine.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class DynamicObject
		{
		private:
			TypeDefinition const* type;

			int refCount = 0;
		public:
			DynamicObject(TypeDefinition const* type) { this->type = type; }
			inline TypeDefinition const* GetType() const { return this->type; };

			inline bool operator<(const DynamicObject* rhs) 
			{
				return this->LessThan(rhs);
			}

			virtual bool LessThan(DynamicObject const* other)
			{
				return this < other;
			}

			virtual bool EqualsTo(DynamicObject const* other)
			{
				return !(this < other) && !(this > other);
			}

			void AddRef(VirtualMachine& vm);
			void RemoveRef(VirtualMachine& vm);
			inline void GetRefCount();

			virtual DynamicObject* Copy(VirtualMachine& vm, DynamicObject* object);
		};
	}
}