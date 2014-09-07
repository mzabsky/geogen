#pragma once

#include <deque>
#include "../CodeLocation.hpp"
#include "../Serializable.hpp"

namespace geogen
{
	namespace runtime
	{
		class CallStackEntry;
		class MemoryManager;
		class FunctionDefinition;

		/// Data structure containing all required information about function call stack. Each call
		/// CallStackEntry then contains a code block stack.
		class CallStack : public Serializable
		{
			std::deque<CallStackEntry*> stack;

			// Non-copyable
			CallStack(CallStack const&) {};
			CallStack& operator=(CallStack const&) {};
		public:
			static const unsigned SIZE_LIMIT;

			/// Initializes a new instance of the CallStack class.
			CallStack() {};

			/// Destroys all CallStackEntry items owned by this instance.
			~CallStack();

			typedef std::deque<CallStackEntry*>::const_iterator const_iterator;
			typedef std::deque<CallStackEntry*>::iterator iterator;
			typedef std::deque<CallStackEntry*>::const_reverse_iterator const_reverse_iterator;
			typedef std::deque<CallStackEntry*>::reverse_iterator reverse_iterator;

			/// Gets the topmost entry in the stack.
			/// @return A reference to the topmost entry in the stack.
			CallStackEntry& Top();
			
			/// Gets the topmost entry in the stack.
			/// @return A reference to the topmost entry in the stack.
			CallStackEntry const& Top() const;

			/// Removes the topmost object from the stack.
			void Pop();

			/// Pushes an object onto this stack.
			/// @param location The code location.
			/// @param functionDefinition The function definition.
			void Push(CodeLocation location, FunctionDefinition const* functionDefinition);

			/// Returns a value indicating whether this CallStack is empty.
			/// @return true if empty, false if not.
			inline bool IsEmpty() const { return this->stack.empty(); };

			inline const_iterator Begin() const { return *(const_iterator*)(&this->stack.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->stack.end()); }

			inline iterator Begin() { return this->stack.begin(); }
			inline iterator End() { return this->stack.end(); }

			inline const_reverse_iterator RBegin() const { return *(const_reverse_iterator*)(&this->stack.rbegin()); }
			inline const_reverse_iterator REnd() const { return *(const_reverse_iterator*)(&this->stack.rend()); }

			inline reverse_iterator RBegin() { return this->stack.rbegin(); }
			inline reverse_iterator REnd() { return this->stack.rend(); }

			virtual void Serialize(IOStream& stream) const;
		};
	}
}