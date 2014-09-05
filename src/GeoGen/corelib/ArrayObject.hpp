#pragma once

#include <vector>
#include <list>

#include "../runtime/ManagedObject.hpp"
#include "../random/RandomSeed.hpp"

namespace geogen
{
	namespace corelib
	{
		class ArrayObject : public runtime::ManagedObject
		{
		public:
			typedef std::map<ManagedObject*, ManagedObject*, geogen::runtime::CompareObjects> HashMap;
			typedef std::list<ManagedObject*> List;
			typedef List::iterator iterator;
			typedef List::const_iterator const_iterator;
			typedef List::reverse_iterator reverse_iterator;
			typedef List::const_reverse_iterator const_reverse_iterator;
		private:
			HashMap hashmap;
			List list;

			int maxIntegerKey;
		public:
			ArrayObject(runtime::VirtualMachine* vm, runtime::TypeDefinition const* type) : ManagedObject(vm, type), maxIntegerKey(-1)
			{
			};

			virtual ~ArrayObject();

			//inline BackingCollection& GetBackingCollection() { return this->hashmap; }
			//inline BackingCollection const& GetBackingCollection() const { return this->hashmap; }

			inline const_iterator Begin() const { return *(const_iterator*)(&this->list.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->list.end()); }

			inline iterator Begin() { return this->list.begin(); }
			inline iterator End() { return this->list.end(); }

			inline const_reverse_iterator RBegin() const { return *(const_reverse_iterator*)(&this->list.rbegin()); }
			inline const_reverse_iterator REnd() const { return *(const_reverse_iterator*)(&this->list.rend()); }

			inline reverse_iterator RBegin() { return this->list.rbegin(); }
			inline reverse_iterator REnd() { return this->list.rend(); }

			inline HashMap::iterator MBegin() { return this->hashmap.begin(); }
			inline HashMap::iterator MEnd() { return this->hashmap.end(); }

			void Set(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key, ManagedObject* value);
			ManagedObject* Get(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key);
			ManagedObject* GetActualKey(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key);

			ManagedObject* Front(runtime::VirtualMachine* vm, CodeLocation location);
			ManagedObject* Back(runtime::VirtualMachine* vm, CodeLocation location);

			//ManagedObject* PushFront(runtime::VirtualMachine* vm, ManagedObject* object);
			void PushBack(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* object);

			void PopFront(runtime::VirtualMachine* vm, CodeLocation location);
			void PopBack(runtime::VirtualMachine* vm, CodeLocation location);

			bool ContainsKey(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key);
			bool ContainsValue(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* value);

			//void Insert(runtime::VirtualMachine* vm, unsigned index, ManagedObject* object);
			//void RemoveAt(runtime::VirtualMachine* vm, CodeLocation location, unsigned index);
			void RemoveKey(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* key);
			void RemoveValue(runtime::VirtualMachine* vm, CodeLocation location, ManagedObject* value);

			inline unsigned Count() const { return this->hashmap.size(); };

			ManagedObject* GetKeyByIndex(runtime::VirtualMachine* vm, CodeLocation location, int index);
			ManagedObject* GetValueByIndex(runtime::VirtualMachine* vm, CodeLocation location, int index);

			void SortByKeys(runtime::VirtualMachine* vm, CodeLocation location);
			void SortByValues(runtime::VirtualMachine* vm, CodeLocation location);

			void Shuffle(runtime::VirtualMachine* vm, CodeLocation location, random::RandomSeed randomSeed);

			virtual String GetStringValue() const;
		};
	}
}