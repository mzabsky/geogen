#pragma once

#include <string>
#include <functional>

#include "../CodeLocation.hpp"
#include "VirtualMachine.hpp"
#include "MemoryManager.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class TypeDefinition;

		class DynamicObject : public Serializable
		{
		private:
			TypeDefinition const* type;

			std::map<std::string, DynamicObject*> memberValues;
			int refCount = 0;

			VariableTable memberVariableTable;
			//bool DefineMemberValue(VirtualMachine& vm, std::string const& name);
		protected:
			DynamicObject(VirtualMachine* vm, TypeDefinition const* type);
		public:			
			virtual ~DynamicObject(){};

			virtual bool IsStaticObject(){ return false; };

			inline TypeDefinition const* GetType() const { return this->type; };

			bool operator<(const DynamicObject* rhs);

			inline VariableTable& GetMemberVariableTable() { return this->memberVariableTable; }

			void AddRef(MemoryManager& vm);
			void RemoveRef(MemoryManager& vm);
			inline int GetRefCount() { return this->refCount; }			

			//void SetMemberValue(VirtualMachine& vm, std::string const& name, DynamicObject* object);
			//bool SetMemberValue(VirtualMachine& vm, CodeLocation location, std::string memberName, DynamicObject* object);

			//DynamicObject* GetMemberValue(VirtualMachine& vm, std::string const& name) const;
			//DynamicObject* GetMemberValue(VirtualMachine& vm, CodeLocation location, std::string memberName) const;

			virtual void Serialize(std::iostream& stream) const = 0;
		};
	}
}