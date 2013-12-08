#include <istream>
#include <iostream>
#include <sstream>

#include "CodeBlock.hpp"
#include "instructions/Instruction.hpp"

using namespace geogen::compiler;

CodeBlock::~CodeBlock() 
{
	for(std::vector<instructions::Instruction const*>::iterator it = this->instructions.begin(); it != this->instructions.end(); it++)
	{
		delete *it;
	}
}

void CodeBlock::AddInstruction(instructions::Instruction const* instruction)
{
	this->instructions.push_back(instruction);
}

void CodeBlock::MoveInstructionsFrom(CodeBlock& another)
{
	for(std::vector<instructions::Instruction const*>::iterator it = another.instructions.begin(); it != another.instructions.end(); it++)
	{
		this->instructions.push_back(*it);
	}

	another.instructions.clear();
}

void CodeBlock::Serialize(std::iostream& stream) const
{
	stream << "{" << std::endl;

	for(const_iterator it = this->Begin(); it != this->End(); it++)
	{
		std::stringstream instructionStream;
		std::string line;

		(*it)->Serialize(instructionStream);

		while (getline(instructionStream, line))
		{
			stream << "\t" << line << std::endl;
		}
	}

	stream << "}" << std::endl;
}
