#pragma once

#include <string>
#include <deque>

#include "../Serializable.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class ManagedObject;

		class ObjectStack : public Serializable
		{
			std::deque<ManagedObject*> stack;

			// Non-copyable
			ObjectStack(ObjectStack const&) {};
			ObjectStack& operator=(ObjectStack const&) {};
		public:
			static const unsigned SIZE_LIMIT;

			ObjectStack() {};
			//~ObjectStack();

			typedef std::deque<ManagedObject*>::const_iterator const_iterator;
			typedef std::deque<ManagedObject*>::iterator iterator;

			ManagedObject* Top();
			void Pop(VirtualMachine* vm);
			void Push(CodeLocation location, ManagedObject* object);
			void CheckSize(unsigned requiredSize);

			inline const_iterator Begin() const { return *(const_iterator*)(&this->stack.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->stack.end()); }

			inline iterator Begin() { return this->stack.begin(); }
			inline iterator End() { return this->stack.end(); }

			virtual void Serialize(std::iostream& stream) const;
		};
	}
}