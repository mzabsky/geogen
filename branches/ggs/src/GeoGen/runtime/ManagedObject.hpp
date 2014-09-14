#pragma once

#include <functional>

#include "../String.hpp"
#include "../CodeLocation.hpp"
#include "MemoryManager.hpp"
#include "ObjectId.hpp"
#include "VariableTable.hpp"
#include "TypeDefinition.hpp"

namespace geogen 
{
	namespace runtime 
	{
		class TypeDefinition;
		class VirtualMachine;

		/// Base class for script objects managed by MemoryManager.
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

			/// Constructor.
			/// @param vm The virtual machine.
			/// @param type The type definition.
			ManagedObject(VirtualMachine* vm, TypeDefinition const* type);
		public:			
			/// Destructor.
			virtual ~ManagedObject();

			/// Determines if the object is an static object (an object representing a type).
			/// @return true if this object is an static object.
			virtual bool IsStaticObject(){ return false; };

			/// Determines if the object represents an reference to another object.
			/// @return true if reference object, false if not.
			virtual bool IsReferenceObject(){ return false; };

			/// Gets the type definition of type of this object.
			/// @return null if it fails, else the type.
			inline TypeDefinition const* GetType() const { return this->type; };

			/// Gets member variable table.
			/// @return The member variable table.
			inline VariableTable& GetMemberVariableTable() { return this->memberVariableTable; }

			/// Adds a reference to the ref-counter of this object.
			void AddRef(/*MemoryManager& vm*/);

			/// Removes a reference from ref-counter of this object. Destroys the object if the ref-count becomes 0.
			void RemoveRef(MemoryManager& vm);

			/// Gets ref-count of this object.
			/// @return The reference count.
			inline int GetRefCount() { return this->refCount; }			

			/// Gets object ID.
			/// @return The object ID.
			inline ObjectId GetObjectId() const { return this->objectId; };

			/// Sets object ID.
			/// @param objectId The object ID.
			inline void SetObjectId(ObjectId objectId) { this->objectId = objectId; };

			/// Gets a string representation of value of this object.
			/// @return The string value.
			virtual String GetStringValue() const = 0;

			virtual void Serialize(IOStream& stream) const;			
		};

		/// Comparator which allows ManagedObject::InstanceLessThan to be used for STL collections.
		struct CompareObjects {

			/// The comparison.
			/// @return true if A is less than B.
			inline bool operator() (ManagedObject* a, ManagedObject* b) const
			{
				return a->GetType()->InstanceLessThan(a, b);
			}
		};
	}
}