#include <iostream>

#include "CodeBlock.hpp"
#include "instructions/Instruction.hpp"

using namespace geogen::compiler;

void CodeBlock::AddInstruction(instructions::Instruction const* instruction)
{
	this->instructions.push_back(instruction);
}