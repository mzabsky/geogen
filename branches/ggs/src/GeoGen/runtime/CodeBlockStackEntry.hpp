#pragma once

#include <map>
#include <stack>

//#include "ManagedObject.hpp"
#include "CodeBlock.hpp"
#include "FunctionDefinition.hpp"
#include "VariableTable.hpp"
#include "instructions/Instruction.hpp"
#include "../Serializable.hpp"
#include "../CodeLocation.hpp"

namespace geogen
{
	namespace runtime
	{
		class ManagedObject;
		class VirtualMachine;

		enum CodeBlockStackEntryStepResult
		{
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_RUNNING,
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED,
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_BREAK,
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE
		};

		class CodeBlockStackEntry : public Serializable
		{
		private:
			CodeBlock const* codeBlock;
			CodeBlock::const_iterator codePointer;
			bool isLooping;
			VariableTable localVariableTable;
			CodeLocation location;

			CodeBlockStackEntry(CodeBlockStackEntry const& other);
			CodeBlockStackEntry& operator=(CodeBlockStackEntry const& other);
		public:			
			CodeBlockStackEntry(CodeLocation location, MemoryManager* memoryManager, CodeBlock const& codeBlock, bool isLooping);
			~CodeBlockStackEntry() {};

			inline bool IsLooping() const { return this->isLooping; };

			inline CodeLocation GetLocation() const { return this->location; };
			inline CodeBlock const& GetCodeBlock() const { return *this->codeBlock; };
			inline VariableTable& GetLocalVariableTable() { return this->localVariableTable; };

			const instructions::Instruction* GetCurrentInstruction() const;
			CodeBlockStackEntryStepResult Step(VirtualMachine* vm);

			virtual void Serialize(IOStream& stream) const;
		};
	}
}