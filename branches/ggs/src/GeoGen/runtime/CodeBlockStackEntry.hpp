#pragma once

#include <map>
#include <string>
#include <stack>

//#include "DynamicObject.hpp"
#include "CodeBlock.hpp"
#include "FunctionDefinition.hpp"
#include "instructions/Instruction.hpp"
#include "../Serializable.hpp"

namespace geogen
{
	namespace runtime
	{
		class DynamicObject;
		class VirtualMachine;

		enum CodeBlockStackEntryStepResult
		{
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_RUNNING,
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_FINISHED,
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_BREAK,
			CODE_BLOCK_STACK_ENTRY_STEP_RESULT_TYPE_CONTINUE
		};

		/*struct CodeBlockStackEntryStepResult
		{
			CodeBlockStackEntryStepResultType type;
			unsigned codeBlockCount;
			bool isLooping;
		};*/

		class CodeBlockStackEntry : public Serializable
		{
		private:
			CodeBlock const* codeBlock;
			CodeBlock::const_iterator codePointer;
			bool isLooping;
			std::map<std::string, DynamicObject*> localVariableValues;
		public:
			CodeBlockStackEntry(CodeBlockStackEntry const& other);
			CodeBlockStackEntry& operator=(CodeBlockStackEntry const& other);

			CodeBlockStackEntry(CodeBlock const& codeBlock, bool isLooping);
			~CodeBlockStackEntry() {};

			inline bool IsLooping() const { return this->isLooping; };

			inline CodeBlock const& GetCodeBlock() const { return *this->codeBlock; };
			const instructions::Instruction* GetCurrentInstruction() const;
			CodeBlockStackEntryStepResult Step(VirtualMachine* vm);

			virtual void Serialize(std::iostream& stream) const;
		};
	}
}