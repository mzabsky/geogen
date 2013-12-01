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
		public:		
			typedef std::vector<instructions::Instruction const*>::const_iterator const_iterator;

			void AddInstruction(instructions::Instruction const* instruction);

			inline const_iterator Begin() const { return this->instructions.begin(); }
			inline const_iterator End() const { return this->instructions.end(); }
		};
	}
}