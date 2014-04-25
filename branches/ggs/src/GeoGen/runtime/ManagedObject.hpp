#pragma once

#include <string>
#include <functional>

#include "../CodeLocation.hpp"
#include "VirtualMachine.hpp"
#include "MemoryManager.hpp"
#include "ObjectId.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class TypeDefinition;

		class ManagedObject : public Serializable
		{
		private:			
			TypeDefinition const* type;
			ObjectId objectId;
			std::map<std::string, ManagedObject*> memberValues;
			int refCount = 0;

			VariableTable memberVariableTable;
			//bool DefineMemberValue(VirtualMachine& vm, std::string const& name);			
		protected:
			ManagedObject(VirtualMachine* vm, TypeDefinition const* type);
		public:			
			virtual ~ManagedObject(){};

			virtual bool IsStaticObject(){ return false; };

			inline TypeDefinition const* GetType() const { return this->type; };

			bool operator<(const ManagedObject* rhs);

			inline VariableTable& GetMemberVariableTable() { return this->memberVariableTable; }

			void AddRef(MemoryManager& vm);
			void RemoveRef(MemoryManager& vm);
			inline int GetRefCount() { return this->refCount; }			

			inline ObjectId GetObjectId() { return this->objectId; };
			inline void SetObjectId(ObjectId objectId) { this->objectId; };

			//void SetMemberValue(VirtualMachine& vm, std::string const& name, ManagedObject* object);
			//bool SetMemberValue(VirtualMachine& vm, CodeLocation location, std::string memberName, ManagedObject* object);

			//ManagedObject* GetMemberValue(VirtualMachine& vm, std::string const& name) const;
			//ManagedObject* GetMemberValue(VirtualMachine& vm, CodeLocation location, std::string memberName) const;

			virtual std::string GetStringValue() const = 0;
			virtual void Serialize(std::iostream& stream) const;
		};
	}
}