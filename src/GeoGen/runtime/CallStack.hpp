#pragma once

#include <string>
#include <deque>
#include "../CodeLocation.hpp"

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
			typedef std::deque<CallStackEntry*>::const_reverse_iterator const_reverse_iterator;
			typedef std::deque<CallStackEntry*>::reverse_iterator reverse_iterator;

			CallStackEntry& Top();
			void Pop();
			void Push(CodeLocation location, FunctionDefinition const* functionDefinition);
			inline bool IsEmpty() { return this->stack.empty(); };

			inline const_iterator Begin() const { return *(const_iterator*)(&this->stack.begin()); }
			inline const_iterator End() const { return *(const_iterator*)(&this->stack.end()); }

			inline iterator Begin() { return this->stack.begin(); }
			inline iterator End() { return this->stack.end(); }

			inline const_reverse_iterator RBegin() const { return *(const_reverse_iterator*)(&this->stack.rbegin()); }
			inline const_reverse_iterator REnd() const { return *(const_reverse_iterator*)(&this->stack.rend()); }

			inline reverse_iterator RBegin() { return this->stack.rbegin(); }
			inline reverse_iterator REnd() { return this->stack.rend(); }
		};
	}
}