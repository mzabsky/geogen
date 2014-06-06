#pragma once

#include <functional>

#include "../String.hpp"
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
#ifdef DEBUG
			// Protection against memory manager errors.
			static const unsigned MANAGED_OBJECT_MAGIC = 0x66E7u;
			unsigned magic = MANAGED_OBJECT_MAGIC;
#endif

			TypeDefinition const* type;
			ObjectId objectId;
			unsigned refCount = 0;

			VariableTable memberVariableTable;
		protected:
			ManagedObject(VirtualMachine* vm, TypeDefinition const* type);
		public:			
			virtual ~ManagedObject();

			virtual bool IsStaticObject(){ return false; };

			inline TypeDefinition const* GetType() const { return this->type; };

			inline VariableTable& GetMemberVariableTable() { return this->memberVariableTable; }

			void AddRef(/*MemoryManager& vm*/);
			void RemoveRef(MemoryManager& vm);
			inline int GetRefCount() { return this->refCount; }			

			inline ObjectId GetObjectId() { return this->objectId; };
			inline void SetObjectId(ObjectId objectId) { this->objectId = objectId; };

			virtual String GetStringValue() const = 0;
			virtual void Serialize(std::iostream& stream) const;			
		};

		struct CompareObjects {
			inline bool operator() (ManagedObject* a, ManagedObject* b) const
			{
				return a->GetType()->InstanceLessThan(a, b);
			}
		};
	}
}