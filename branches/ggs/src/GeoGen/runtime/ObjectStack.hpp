#pragma once

#include <string>
#include <deque>

#include "../Serializable.hpp"

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;

		class ObjectStack : public Serializable
		{
			std::deque<DynamicObject*> stack;

			// Non-copyable
			ObjectStack(ObjectStack const&) {};
			ObjectStack& operator=(ObjectStack const&) {};
		public:
			ObjectStack() {};
			//~ObjectStack();

			typedef std::deque<DynamicObject*>::const_iterator const_iterator;
			typedef std::deque<DynamicObject*>::iterator iterator;

			DynamicObject* Top();
			void Pop();
			void Push(DynamicObject* object);
			void CheckSize(unsigned requiredSize);

			inline const_iterator Begin() const { return *(const_iterator*)(&this->stack.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->stack.end()); }

			inline iterator Begin() { return this->stack.begin(); }
			inline iterator End() { return this->stack.end(); }

			virtual void Serialize(std::iostream& stream) const;
		};
	}
}