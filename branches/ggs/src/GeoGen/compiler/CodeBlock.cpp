#include <iostream>

#include "CodeBlock.hpp"
#include "instructions/Instruction.hpp"

using namespace geogen::compiler;

void CodeBlock::AddInstruction(const instructions::Instruction& instruction)
{
	this->instructions.push_back(instruction);
}