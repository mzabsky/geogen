#pragma once

#include <string>
#include <functional>

#include "../CodeLocation.hpp"
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

			std::map<int, DynamicObject*> memberValues;
			int refCount = 0;

			void DefineMemberValue(VirtualMachine& vm, std::string const& name);
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

			//void SetMemberValue(VirtualMachine& vm, std::string const& name, DynamicObject* object);
			void SetMemberValue(VirtualMachine& vm, CodeLocation location, int memberNameIndex, DynamicObject* object);

			//DynamicObject* GetMemberValue(VirtualMachine& vm, std::string const& name) const;
			DynamicObject* GetMemberValue(VirtualMachine& vm, CodeLocation location, int memberNameIndex) const;
		};
	}
}