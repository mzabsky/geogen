#pragma once

#include <string>
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

		/// <summary>
		/// Data structure containing all required information about function call stack. Each call <see>CallStackEntry</see> then contains a code block stack.
		/// </summary>
		class CallStack : public Serializable
		{
			std::deque<CallStackEntry*> stack;

			// Non-copyable
			CallStack(CallStack const&) {};
			CallStack& operator=(CallStack const&) {};
		public:
			static const unsigned SIZE_LIMIT;

			/// <summary>
			/// Initializes a new instance of the <see cref="CallStack"/> class.
			/// </summary>
			CallStack() {};

			/// <summary>
			/// Destroys all <see>CallStackEntry</see> items owned by this instance.
			/// </summary>
			~CallStack();

			typedef std::deque<CallStackEntry*>::const_iterator const_iterator;
			typedef std::deque<CallStackEntry*>::iterator iterator;
			typedef std::deque<CallStackEntry*>::const_reverse_iterator const_reverse_iterator;
			typedef std::deque<CallStackEntry*>::reverse_iterator reverse_iterator;

			/// <summary> Gets the topmost entry in the stack. </summary>
			/// <returns> A reference to the topmost entry in the stack.</returns>
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

			virtual void Serialize(std::iostream& stream) const;
		};
	}
}