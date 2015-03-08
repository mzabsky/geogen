#pragma once

#include <deque>

#include "../Serializable.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace runtime
	{
		class VirtualMachine;
		class ManagedObject;

		/// Data structure containing in-expression objects and objects being passed into/from a function. 
		class ObjectStack : public Serializable
		{
			std::deque<ManagedObject*> stack;

			// Non-copyable
			ObjectStack(ObjectStack const&) {};
			ObjectStack& operator=(ObjectStack const&) {};
		public:
			/// Maximum number of items allowed to be on the stack.
			static const unsigned SIZE_LIMIT;

			/// Default constructor.
			ObjectStack() {};
			//~ObjectStack();

			/// A constant iterator.
			typedef std::deque<ManagedObject*>::const_iterator const_iterator;

			/// An iterator.
			typedef std::deque<ManagedObject*>::iterator iterator;

			/// A constant reverse iterator.
			typedef std::deque<ManagedObject*>::const_reverse_iterator const_reverse_iterator;

			/// An reverse iterator.
			typedef std::deque<ManagedObject*>::reverse_iterator reverse_iterator;

			/// Gets the topmost entry in the stack. Throws an exception if the stack is empty.
			/// @return A reference to the topmost entry in the stack.
			ManagedObject* Top();

			/// Removes the topmost object from the stack. Throws an exception if the stack is empty.
			void Pop(VirtualMachine* vm);

			/// Pushes an object onto this stack.
			/// @param location The code location.
			/// @param object The object.
			void Push(CodeLocation location, ManagedObject* object);

			/// Gets number of items on the stack.
			/// @return Size of the stack.
			inline size_t Size() const { return this->stack.size(); };

			/// Checks if the stack contains at least given number of items. Throws exception if there are fewer items on the stack.
			/// @param requiredSize The number of items expected to be on the stack.
			void CheckSize(unsigned requiredSize);

			/// Gets an iterator pointing to the first (bottommost) item on the stack or the end iterator, if the stack is empty.
			/// @return An iterator.
			inline const_iterator Begin() const { return *(const_iterator*)(&this->stack.begin()); }

			/// Gets the end iterator.
			/// @return An iterator.
			inline const_iterator End() const { return *(const_iterator*)(&this->stack.end()); }

			/// Gets an iterator pointing to the first (bottommost) item on the stack.
			/// @return An iterator.
			inline iterator Begin() { return this->stack.begin(); }

			/// Gets the end iterator.
			/// @return An iterator.
			inline iterator End() { return this->stack.end(); }

			/// Gets a reverse iterator pointing to the last (topmost) item on the stack.
			/// @return An iterator.
			inline const_reverse_iterator RBegin() const { return *(const_reverse_iterator*)(&this->stack.rbegin()); }

			/// Gets the reverse end iterator.
			/// @return An iterator.
			inline const_reverse_iterator REnd() const { return *(const_reverse_iterator*)(&this->stack.rend()); }

			/// Gets a reverse iterator pointing to the last (topmost) item on the stack.
			/// @return An iterator.
			inline reverse_iterator RBegin() { return this->stack.rbegin(); }

			/// Gets the reverse end iterator.
			/// @return An iterator.
			inline reverse_iterator REnd() { return this->stack.rend(); }

			virtual void Serialize(IOStream& stream) const;
		};
	}
}