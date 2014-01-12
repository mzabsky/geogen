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
		protected:
			DynamicObject(TypeDefinition const* type);			
		public:			
			virtual ~DynamicObject(){};

			inline TypeDefinition const* GetType() const { return this->type; };

			inline bool operator<(const DynamicObject* rhs) 
			{
				return this->GetType()->InstanceLessThan(this, rhs);
			}

			void AddRef(VirtualMachine& vm);
			void RemoveRef(VirtualMachine& vm);
			inline int GetRefCount() { return this->refCount; }
		};
	}
}