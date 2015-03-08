#pragma once

#include <deque>

#include "../CodeLocation.hpp"
#include "CodeBlock.hpp"

namespace geogen
{
	namespace runtime
	{
		class CodeBlockStackEntry;
		class MemoryManager;

		/// Data structure containing a recursive sequence of calls to code block.Each CallStackEntry usually contains such stack.
		class CodeBlockStack : public Serializable
		{
			std::deque<CodeBlockStackEntry*> stack;

			// Non-copyable
			CodeBlockStack(CodeBlockStack const&) {};
			CodeBlockStack& operator=(CodeBlockStack const&) {};
		public:
			/// Maximum number of items allowed to be on the stack.
			static const unsigned SIZE_LIMIT;

			/// Default constructor.
			CodeBlockStack() {};

			/// Destructor.
			~CodeBlockStack();

			/// A constant iterator.
			typedef std::deque<CodeBlockStackEntry*>::const_iterator const_iterator;

			/// An iterator.
			typedef std::deque<CodeBlockStackEntry*>::iterator iterator;

			/// A constant reverse iterator.
			typedef std::deque<CodeBlockStackEntry*>::const_reverse_iterator const_reverse_iterator;

			/// An reverse iterator.
			typedef std::deque<CodeBlockStackEntry*>::reverse_iterator reverse_iterator;

			/// Gets the topmost entry in the stack. Throws an exception if the stack is empty.
			/// @return A reference to the topmost entry in the stack.
			CodeBlockStackEntry& Top();

			/// Gets the topmost entry in the stack. Throws an exception if the stack is empty.
			/// @return A reference to the topmost entry in the stack.
			CodeBlockStackEntry const& Top() const;

			/// Removes the topmost entry from the stack. Throws an exception if the stack is empty.
			void Pop();

			/// Pushes an code block entry onto this stack.
			/// @param location The code location.
			/// @param memoryManager The memory manager.
			/// @param codeBlock The code block.
			/// @param isLooping true if the code block is to be executed in a loop.
			void Push(CodeLocation location, MemoryManager* memoryManager, CodeBlock const& codeBlock, bool isLooping);

			/// Gets number of items on the stack.
			/// @return Size of the stack.
			inline size_t Size() const { return this->stack.size(); };

			/// Checks if the stack contains at least given number of items. Throws exception if there are fewer items on the stack.
			/// @param requiredSize The number of items expected to be on the stack.
			void CheckSize(unsigned requiredSize) const;

			/// Returns a value indicating whether this CallStack is empty.
			/// @return true if empty, false if not.
			inline bool IsEmpty() const { return this->stack.empty(); };

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