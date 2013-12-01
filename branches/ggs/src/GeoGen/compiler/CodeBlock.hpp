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
			std::vector<instructions::Instruction> instructions;
		public:		
			typedef std::vector<instructions::Instruction>::const_iterator const_iterator;

			void AddInstruction(const instructions::Instruction& instruction);

			inline const_iterator Begin() const { return this->instructions.begin(); }
			inline const_iterator End() const { return this->instructions.end(); }
		};
	}
}