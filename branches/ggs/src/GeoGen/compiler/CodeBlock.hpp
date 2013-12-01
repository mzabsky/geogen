#pragma once

#include <vector>

#include "instructions/Instruction.hpp"

namespace geogen 
{
	namespace compiler 
	{
		class CodeBlock
		{
		private:
			std::vector<instructions::Instruction const*> instructions;
			
			CodeBlock(CodeBlock const& other) {};
			CodeBlock& operator=(CodeBlock const&) {};
		public:		
			CodeBlock() {};			
			~CodeBlock();

			typedef std::vector<instructions::Instruction const*>::const_iterator const_iterator;

			void AddInstruction(instructions::Instruction const* instruction);
			void MoveInstructionsFrom(CodeBlock& another);

			inline const_iterator Begin() const { return this->instructions.begin(); }
			inline const_iterator End() const { return this->instructions.end(); }
		};
	}
}