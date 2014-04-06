#pragma once

#include <string>
#include <deque>

namespace geogen
{
	namespace runtime
	{
		class CallStackEntry;
		class MemoryManager;
		class FunctionDefinition;

		class CallStack
		{
			std::deque<CallStackEntry*> stack;

			// Non-copyable
			CallStack(CallStack const&) {};
			CallStack& operator=(CallStack const&) {};
		public:
			CallStack() {};
			~CallStack();

			typedef std::deque<CallStackEntry*>::const_iterator const_iterator;
			typedef std::deque<CallStackEntry*>::iterator iterator;

			CallStackEntry& Top();
			void Pop();
			void Push(FunctionDefinition const* functionDefinition);
			inline bool IsEmpty() { return this->stack.empty(); };

			inline const_iterator Begin() const { return *(const_iterator*)(&this->stack.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->stack.end()); }

			inline iterator Begin() { return this->stack.begin(); }
			inline iterator End() { return this->stack.end(); }
		};
	}
}