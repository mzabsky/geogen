#pragma once

#include <string>
#include <deque>

#include "CodeBlock.hpp"

namespace geogen
{
	namespace runtime
	{
		class CodeBlockStackEntry;
		class MemoryManager;

		class CodeBlockStack : public Serializable
		{
			std::deque<CodeBlockStackEntry*> stack;

			// Non-copyable
			CodeBlockStack(CodeBlockStack const&) {};
			CodeBlockStack& operator=(CodeBlockStack const&) {};
		public:
			CodeBlockStack() {};
			~CodeBlockStack();

			typedef std::deque<CodeBlockStackEntry*>::const_iterator const_iterator;
			typedef std::deque<CodeBlockStackEntry*>::iterator iterator;
			typedef std::deque<CodeBlockStackEntry*>::const_reverse_iterator const_reverse_iterator;
			typedef std::deque<CodeBlockStackEntry*>::reverse_iterator reverse_iterator;

			CodeBlockStackEntry& Top();
			void Pop();
			void Push(MemoryManager* memoryManager, CodeBlock const& codeBlock, bool isLooping);
			inline bool IsEmpty() { return this->stack.empty(); };
			inline size_t Size() { return this->stack.size(); };

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